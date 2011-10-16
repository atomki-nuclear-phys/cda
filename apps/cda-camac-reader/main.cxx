// $Id$
/**
 *   @file apps/cda-camac-reader/main.cxx
 *  @short Main file for the CAMAC crate reader application
 *
 *         This file stores the code that runs the cda-camac-reader application.
 *         The file is a bit long, but I didn't want to distribute its code.
 *         I think that would've only made it more difficult to understand...
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
#   include "cdacore/camac/Crate.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/Sender.h"
#   include "cdacore/i18n/Loader.h"
#   include "cdadaq/stat/Sender.h"
#   include "cdadaq/config/ConfReader.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "device/Loader.h"
#   include "camac/Crate.h"
#   include "event/Event.h"
#   include "event/Sender.h"
#   include "i18n/Loader.h"
#   include "stat/Sender.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"

// Function forward declaration(s):
void shutDown( int );
// Global variable(s):
static msg::Logger   g_logger( "cda-camac-reader" );
static camac::Crate* g_crate = 0;

// Description for the executable:
static const char* description =
   "Program reading events from a CAMAC crate for the CDA\n"
   "application suit.\n\n"
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
   CmdArgStrList msgservers( 'm', "msgservers", "addresses",
                             "Addresses of message servers" );
   CmdArgStrList clients( 'e', "clients", "addresses",
                          "Addresses of event reader clients" );
   CmdArgStrList statistics( 's', "statistics", "addresses",
                             "Addresses of statistics reader clients" );

   CmdLine cmd( *argv, &verbosity, &config, &msgservers, &clients,
                &statistics, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   cmd.parse( arg_iter );

   //
   // Set the destination of the messages:
   //
   for( unsigned int i = 0; i < msgservers.count(); ++i ) {
      msg::Sender::addAddress( Address( ( const char* ) msgservers[ i ] ) );
   }

   //
   // Load all the available translations:
   //
   QCoreApplication app( argc, argv );
   i18n::Loader trans_loader;
   if( ! trans_loader.loadTranslations() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-camac-reader",
                                   "Couldn't load the translations!" )
               << msg::endmsg;
      return 1;
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
      g_logger << msg::FATAL
               << qApp->translate( "cda-camac-reader",
                                   "Didn't recognise verbosity level setting\n"
                                   "Terminating..." )
               << msg::endmsg;
      return 1;
   }

   //
   // Try to load all available device plugins:
   //
   dev::Loader loader;
   if( loader.loadAll() ) {
      g_logger << msg::INFO
               << qApp->translate( "cda-camac-reader",
                                   "Successfully loaded all available devices" )
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL
               << qApp->translate( "cda-camac-reader",
                                   "There was an error loading the devices" )
               << msg::endmsg;
   }

   //
   // Create the crate object:
   //
   reader::Crate crate;
   crate.setLoader( &loader );

   //
   // Decide how to read the configuration:
   //
   if( Address::isAddress( ( const char* ) config ) ) {

      //
      // Read the configuration data from the specified address:
      //
      conf::ConfReader reader;
      if( ! reader.readFrom( Address( ( const char* ) config ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "Couldn't read configuration from "
                                      "address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      }

      //
      // Initialise the crate object from the buffer:
      //
      if( ! crate.readConfig( reader.buffer() ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "Couldn't process configuration "
                                      "coming from address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-camac-reader",
                                      "Read the configuration from: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

   } else {

      //
      // Open the configuration file:
      //
      QFile config_file( ( const char* ) config );
      if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "The specified configuration file (\"%1\")\n"
                                      "could not be opened!" )
            .arg( ( const char* ) config ? ( const char* ) config : "" )
                  << msg::endmsg;
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
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "Error in parsing \"%1\"\n"
                                      "  Error message: %2\n"
                                      "  Error line   : %3\n"
                                      "  Error column : %4" )
            .arg( ( const char* ) config ).arg( errorMsg )
            .arg( errorLine ).arg( errorColumn )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate( "cda-camac-reader",
                                      "Successfully parsed: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

      //
      // Initialise a Crate object with this configuration:
      //
      QDomElement work = doc.documentElement();
      if( ! crate.readConfig( work ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "Failed to read configuration file!\n"
                                      "See previous messages for more "
                                      "information..." )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-camac-reader",
                                      "Read the configuration from: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

   }

   //
   // Open the CAMAC crate:
   //
   g_crate = new camac::Crate();
   if( ! g_crate->open() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-camac-reader",
                                   "Failed to open CAMAC crate" )
               << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::INFO
               << qApp->translate( "cda-camac-reader",
                                   "Opened the CAMAC crate" )
               << msg::endmsg;
   }

   //
   // Initialize the CAMAC devices for data acquisition:
   //
   if( ! crate.initialize( *g_crate ) ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-camac-reader",
                                   "Failed to initialise devices for data "
                                   "acquisition" )
               << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-camac-reader",
                                   "Initialised devices for data acquisition" )
               << msg::endmsg;
   }

   //
   // Open connections to all the event recepients:
   //
   ev::Sender ev_sender;
   for( unsigned int i = 0; i < clients.count(); ++i ) {
      if( ! ev_sender.addSocket( Address( ( const char* ) clients[ i ] ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "Couldn't connect to event receiver: %1" )
            .arg( ( const char* ) clients[ i ] )
                  << msg::endmsg;
         shutDown( 0 );
      }
   }

   //
   // Open connections to all the statistics recepients. (Ignore connection errors
   // here, since statistics publishing is not a major concern...)
   //
   stat::Sender stat_sender;
   for( unsigned int i = 0; i < statistics.count(); ++i ) {
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
   QString statSource = "cda-camac-reader:";
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
   g_logger << msg::INFO
            << qApp->translate( "cda-camac-reader",
                                "CAMAC readout running..." )
            << msg::endmsg;

   //
   // Read events and send them on the opened FIFO in an endless loop.
   //
   g_evcount = 0;
   for( ; ; ) {

      ev::Event event = crate.readEvent( *g_crate );
      crate.clear( *g_crate );
      if( ! ev_sender.send( event ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
                                      "Failed to send event to all recepients.\n"
                                      "Event readout can not continue!" )
                  << msg::endmsg;
         shutDown( 0 );
      }

      // Update the statistics information after 10 events were sent out:
      ++g_evcount;
      if( ! ( g_evcount % 10 ) ) {
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
 * This function is the last thing to be called when the program terminates.
 * It can tidy up the open connections and the like.
 */
void shutDown( int ) {

   //
   // We have to close and delete the CAMAC crate object explicitly because
   // if it was up to the system to get rid of it, it would try to do it
   // as one of the last steps. At that time the msg::Sender would already
   // be off-line, so when the object would try to send a message about its
   // destruction, that would crash the application just before it would
   // stop anyway. (GDB is our friend...)
   //
   if( ! g_crate->close() ) {
      g_logger << msg::ERROR
               << qApp->translate( "cda-camac-reader",
                                   "Failed to close CAMAC crate" )
               << msg::endmsg;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-camac-reader",
                                   "Successfully closed CAMAC crate" )
               << msg::endmsg;
   }
   delete g_crate;

   g_logger << msg::INFO
            << qApp->translate( "cda-camac-reader",
                                "Total number of events read: %1" )
      .arg( g_evcount )
            << msg::endmsg;
   g_logger << msg::INFO
            << qApp->translate( "cda-camac-reader",
                                "Terminating application..." )
            << msg::endmsg;
   exit( 0 );

   return;
}
