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
#   include "cdacore/i18n/Loader.h"
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
#   include "i18n/Loader.h"
#   include "stat/Sender.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"
#include "GlomemWriter.h"

// Function forward declaration(s):
void shutDown( int );

// Global variable(s):
static msg::Logger g_logger( "cda-glomem-writer" ); ///< Log writer object
static glomem::GlomemWriter* g_gwriter = 0; ///< Global memory writer object

// Description for the executable:
static const char* description =
   "Program writing events which it receives, to a global memory "
   "block of PAW. This makes it possible to monitor the data-taking "
   "from within PAW++ while the data acquisition is on-going.\n\n"
   "This executable should normally be started by CDA internally. "
   "You should only start it by hand for debugging purposes.";

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
               << qApp->translate( "cda-glomem-writer",
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
               << qApp->translate( "cda-glomem-writer",
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
               << qApp->translate( "cda-glomem-writer",
                                   "Successfully loaded all available devices" )
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL
               << qApp->translate( "cda-glomem-writer",
                                   "There was an error loading the devices" )
               << msg::endmsg;
   }

   //
   // Create the crate object:
   //
   glomem::Crate crate;
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
                  << qApp->translate( "cda-glomem-writer",
                                      "Couldn't read configuration from address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      }

      //
      // Initialise the crate object from the buffer:
      //
      if( ! crate.readConfig( reader.buffer() ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "Couldn't process configuration coming from address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-glomem-writer",
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
                  << qApp->translate( "cda-glomem-writer",
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
                  << qApp->translate( "cda-glomem-writer",
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
                  << qApp->translate( "cda-glomem-writer",
                                      "Successfully parsed: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }
      QDomElement work = doc.documentElement();

      //
      // Initialise a Crate object with this configuration:
      //
      if( ! crate.readConfig( work ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "Failed to read configuration file!\n"
                                      "See previous messages for more information..." )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-glomem-writer",
                                      "Read the configuration from: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

   }

   //
   // Initialise the monitoring histograms:
   //
   if( ! crate.initialize() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-glomem-writer",
                                   "Failed to initialise histograms for data "
                                   "acquisition" )
               << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-glomem-writer",
                                   "Initialised histograms for data acquisition" )
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
   cdastat::Sender stat_sender;
   for( unsigned int i = 0; i < statistics.count(); ++i ) {
      stat_sender.addReceiver( Address( ( const char* ) statistics[ i ] ) );
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );
   signal( SIGTERM, shutDown );

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
   stat_sender.update( cdastat::Statistics( 0, statSource ) );
   stat_sender.start();

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO
            << qApp->translate( "cda-glomem-writer",
                                "Histogram writing running..." )
            << msg::endmsg;

   //
   // Create the thread that will take care of writing the monitoring histograms:
   //
   g_gwriter = new glomem::GlomemWriter( evserver, crate );

   //
   // Start the histogram writing and monitoring (this) threads:
   //
   g_gwriter->start();
   for( ; ; ) {

      // Check if the file writing thread is still running:
      if( ! g_gwriter->isRunning() ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "The histogram writing thread unexpectedly died" )
                  << msg::endmsg;
         shutDown( 0 );
      }

      // Update the statistics receivers:
      stat_sender.update( cdastat::Statistics( g_gwriter->processedEvents(),
                                            statSource ) );

      // Sleep for 2 seconds:
      sleep( 2 );
   }

   //
   // The code is never supposed to reach this statement, but it's nicer
   // to have the compiler satisfied...
   //
   return 0;

}

/**
 * This function is called when the application receives an interrupt
 * signal. It stops the event reading thread, displays some summary
 * information about the event processing, and then quits.
 */
void shutDown( int ) {

   // Stop the event processing thread:
   g_gwriter->stopProcessing();

   g_logger << msg::INFO
            << qApp->translate( "cda-glomem-writer",
                                "Total number of events processed: %1" )
      .arg( g_gwriter->processedEvents() )
            << msg::endmsg;
   g_logger << msg::INFO
            << qApp->translate( "cda-glomem-writer",
                                "Terminating application..." )
            << msg::endmsg;

   // Clean up after ourselves:
   delete g_gwriter;

   exit( 0 );

   return;
}
