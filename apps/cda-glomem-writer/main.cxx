// $Id$
/**
 *   @file apps/cda-glomem-writer/main.cxx
 *  @short Main file for the monitoring histogram application
 *
 *         This file stores the code that runs the cda-glomem-writer application.
 *         This application is used to fill monitoring histograms with the
 *         data read from the CAMAC devices.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// System include(s):
#include <cstdlib>
#include <csignal>

// STL include(s):
#include <iostream>
#include <map>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/EventServer.h"
#   include "cdadaq/stat/Sender.h"
#   include "cdadaq/config/ConfReader.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "device/Loader.h"
#   include "fifo/Fifo.h"
#   include "event/Event.h"
#   include "event/EventServer.h"
#   include "stat/Sender.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"

// Function forward declaration(s):
void shutDown( int );

// Global variable(s):
msg::Logger g_logger( "cda-glomem-writer" );

// Description for the executable:
static const char* description =
   "Program writing events which it receives, to a global memory\n"
   "block of PAW. This makes it possible to monitor the data-taking\n"
   "from within PAW++ while the data acquisition is on-going.\n\n"
   "This executable should normally be started by CDA internally.\n"
   "You should only start it by hand for debugging purposes.";

/// Number of events processed
quint32 g_evcount = 0;

int main( int argc, char* argv[] ) {

   //
   // Read the command line options:
   //
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStr config( 'c', "config", "filename/address",
                     "Name of an XML config file or address of a config server",
                     CmdArg::isREQ );
   CmdArgStrList msgservers( 'm', "msgservers", "addresses", "Addresses of message servers" );
   CmdArgStr evaddress( 'e', "evaddress", "address", "Address where to receive events",
                        CmdArg::isREQ );
   CmdArgStrList statistics( 's', "statistics", "addresses",
                             "Addresses of statistics reader clients" );

   CmdLine cmd( *argv, &verbosity, &config, &msgservers, &evaddress,
                &statistics, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   cmd.parse( arg_iter );

   //
   // Set the destination of the messages:
   //
   for( int i = 0; i < msgservers.count(); ++i ) {
      msg::Sender::addAddress( Address( ( const char* ) msgservers[ i ] ) );
   }

   //
   // Translate the verbosity option:
   //
   std::map< int, msg::Level > v_map;
   v_map[ 1 ] = msg::VERBOSE;
   v_map[ 2 ] = msg::DEBUG;
   v_map[ 3 ] = msg::INFO;
   v_map[ 4 ] = msg::WARNING;
   v_map[ 5 ] = msg::ERROR;
   v_map[ 6 ] = msg::FATAL;
   v_map[ 7 ] = msg::ALWAYS;
   if( v_map.find( verbosity ) != v_map.end() ) {
      msg::Sender::instance()->setMinLevel( v_map.find( verbosity )->second );
   } else {
      g_logger << msg::FATAL << "Didn't recognise verbosity level setting"
               << std::endl
               << "Terminating..." << msg::endmsg;
      return 1;
   }

   //
   // Try to load all available device plugins:
   //
   dev::Loader loader;
   if( loader.loadAll() ) {
      g_logger << msg::INFO << "Successfully loaded all available devices"
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL << "There was an error loading the devices"
               << msg::endmsg;
   }

   //
   // Create the crate object:
   //
   glomem::Crate crate;
   crate.setLoader( &loader );

   if( Address::isAddress( ( const char* ) config ) ) {

      conf::ConfReader reader;
      if( ! reader.readFrom( Address( ( const char* ) config ) ) ) {
         g_logger << msg::FATAL << "Couldn't read configuration from address: "
                  << ( const char* ) config << msg::endmsg;
         return 1;
      }

      if( ! crate.readConfig( reader.buffer() ) ) {
         g_logger << msg::FATAL << "Couldn't process configuration coming from address: "
                  << ( const char* ) config << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO << "Read the configuration from: "
                  << ( const char* ) config << msg::endmsg;
      }

   } else {

      //
      // Open the configuration file:
      //
      QFile config_file( ( const char* ) config );
      if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
         g_logger << msg::FATAL << "The specified configuration file (\""
                  << ( ( const char* ) config ? ( const char* ) config : "" )
                  << "\")" << std::endl
                  << "could not be opened!" << msg::endmsg;
         return 1;
      }

      //
      // Read the file's contents into XML format:
      //
      QDomDocument doc;
      QString errorMsg;
      int errorLine, errorColumn;
      if( ! doc.setContent( &config_file, false, &errorMsg, &errorLine,
                            &errorColumn ) ) {
         g_logger << msg::FATAL << "Error in parsing \"" << ( const char* ) config
                  << "\"" << std::endl
                  << "  Error message: " << errorMsg << std::endl
                  << "  Error line   : " << errorLine << std::endl
                  << "  Error column : " << errorColumn << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG << "Successfully parsed: "
                  << ( const char* ) config << msg::endmsg;
      }
      QDomElement work = doc.documentElement();

      //
      // Initialise a Crate object with this configuration:
      //
      if( ! crate.readConfig( work ) ) {
         g_logger << msg::FATAL << "Failed to read configuration file!" << std::endl
                  << "See previous messages for more information..." << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO << "Read the configuration from: "
                  << ( const char* ) config << msg::endmsg;
      }

   }

   //
   // Initialise the monitoring histograms:
   //
   if( ! crate.initialize() ) {
      g_logger << msg::FATAL << "Failed to initialise histograms for data "
               << "acquisition" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Initialised histograms for data acquisition"
               << msg::endmsg;
   }

   //
   // Start an EventServer listening on the specified port:
   //
   ev::EventServer evserver;
   evserver.listen( Address( ( const char* ) evaddress ) );

   //
   // Open connections to all the statistics recepients. (Ignore connection errors
   // here, since statistics publishing is not a major concern...)
   //
   stat::Sender stat_sender;
   for( int i = 0; i < statistics.count(); ++i ) {
      stat_sender.addReceiver( Address( ( const char* ) statistics[ i ] ) );
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );

   //
   // Construct the source string of the statistics objects that are sent out
   // during event processing:
   //
   QString statSource = "cda-glomem-writer:";
   statSource += ( const char* ) config;
   statSource += ":";
   statSource += QString::number( QCoreApplication::applicationPid() );

   // Initialise the statistics information to something meaningful, then start
   // the statistics sender object:
   stat_sender.update( stat::Statistics( 0, statSource ) );
   stat_sender.start();

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO << "Histogram writing running..." << msg::endmsg;

   //
   // Read events and give them to the crate to display, in an endless loop.
   //
   g_evcount = 0;
   for( ; ; ) {

      ev::Event event;
      evserver >> event;

      if( ! crate.displayEvent( event ) ) {
         g_logger << msg::FATAL << "There was a problem diplaying an event"
                  << msg::endmsg;
         shutDown( 0 );
      }

      // Update the statistics information after 10 events were received:
      ++g_evcount;
      if( ! ( g_evcount %10 ) ) {
         stat_sender.update( stat::Statistics( g_evcount, statSource ) );
      }

   }

   //
   // The code is never supposed to reach this statement, but it's nicer
   // to have the compiler satisfied...
   //
   return 0;

}

/**
 * In the case of this application having this shutdown function is a bit of
 * an overkill. But it's nice to have all the CDA applications behaving the
 * same way...
 */
void shutDown( int ) {

   g_logger << msg::INFO << "Total number of events processed: "
            << g_evcount << msg::endmsg;
   g_logger << msg::INFO << "Terminating application..." << msg::endmsg;
   exit( 0 );

   return;

}
