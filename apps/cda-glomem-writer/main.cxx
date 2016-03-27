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
#   include "cdacore/tclap/CmdLine.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/EventServer.h"
#   include "cdacore/i18n/Loader.h"
#   include "cdacore/common/Sleep.h"
#   include "cdadaq/stat/Sender.h"
#   include "cdadaq/config/ConfReader.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "tclap/CmdLine.h"
#   include "device/Loader.h"
#   include "fifo/Fifo.h"
#   include "event/Event.h"
#   include "event/EventServer.h"
#   include "i18n/Loader.h"
#   include "common/Sleep.h"
#   include "stat/Sender.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"
#include "GlomemWriter.h"
#include "../win32/plugins.h"

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
   TCLAP::CmdLine cmd( description );
   TCLAP::ValueArg< int > verbosity( "v", "verbosity",
                                     "Level of output verbosity", false, 3,
                                     "code" );
   cmd.add( verbosity );
   TCLAP::MultiArg< std::string >
         msgservers( "m", "msgservers", "Addresses of message servers",
                     false, "address list" );
   cmd.add( msgservers );
   TCLAP::ValueArg< std::string >
         config( "c", "config", "Name of an XML config file, or "
                 "address of a config server", true, "", "filename/address" );
   cmd.add( config );
   TCLAP::ValueArg< std::string >
         evaddress( "e", "evaddress", "Address on which to receive events",
                    true, "", "address" );
   cmd.add( evaddress );
   TCLAP::MultiArg< std::string >
         statistics( "s", "statistics",
                     "Addresses of statistics reader clients", false,
                     "address list" );
   cmd.add( statistics );
   cmd.parse( argc, argv );

   //
   // Set the destination of the messages:
   //
   for( const std::string& address : msgservers.getValue() ) {
      msg::Sender::addAddress( Address( address.c_str() ) );
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
   auto itr = v_map.find( verbosity.getValue() );
   if( itr != v_map.end() ) {
      msg::Sender::instance()->setMinLevel( itr->second );
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
   if( dev::Loader::instance()->loadAll() ) {
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
   crate.setLoader( dev::Loader::instance() );

   //
   // Decide how to read the configuration:
   //
   if( Address::isAddress( config.getValue().c_str() ) ) {

      //
      // Read the configuration data from the specified address:
      //
      conf::ConfReader reader;
      if( ! reader.readFrom( Address( config.getValue().c_str() ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "Couldn't read configuration from "
                                      "address: %1" )
                     .arg( config.getValue().c_str() )
                  << msg::endmsg;
         return 1;
      }

      //
      // Initialise the crate object from the buffer:
      //
      if( ! crate.readConfig( reader.buffer() ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "Couldn't process configuration coming "
                                      "from address: %1" )
                     .arg( config.getValue().c_str() )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-glomem-writer",
                                      "Read the configuration from: %1" )
                     .arg( config.getValue().c_str() )
                  << msg::endmsg;
      }

   } else {

      //
      // Open the configuration file:
      //
      QFile config_file( config.getValue().c_str() );
      if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "The specified configuration file "
                                      "(\"%1\") could not be opened!" )
                     .arg( config.getValue().c_str() ?
                              config.getValue().c_str() : "" )
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
                     .arg( config.getValue().c_str() ).arg( errorMsg )
                     .arg( errorLine ).arg( errorColumn )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate( "cda-glomem-writer",
                                      "Successfully parsed: %1" )
                     .arg( config.getValue().c_str() )
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
                                      "See previous messages for more "
                                      "information..." )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-glomem-writer",
                                      "Read the configuration from: %1" )
                     .arg( config.getValue().c_str() )
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
                                   "Initialised histograms for data "
                                   "acquisition" )
               << msg::endmsg;
   }

   //
   // Start an EventServer listening on the specified port:
   //
   ev::EventServer evserver;
   evserver.listen( Address( evaddress.getValue().c_str() ) );

   //
   // Open connections to all the statistics recepients. (Ignore connection
   // errors here, since statistics publishing is not a major concern...)
   //
   cdastat::Sender stat_sender;
   for( const std::string& server : statistics.getValue() ) {
      stat_sender.addReceiver( Address( server.c_str() ) );
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
   statSource += config.getValue().c_str();
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

      // Check if the histogram writing thread is still running:
      if( ! g_gwriter->isRunning() ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-glomem-writer",
                                      "The histogram writing thread "
                                      "unexpectedly died" )
                  << msg::endmsg;
         shutDown( 0 );
      }

      // Update the statistics receivers:
      stat_sender.update( cdastat::Statistics( g_gwriter->processedEvents(),
                                               statSource ) );

      // Sleep for 2 seconds:
      common::Sleep( 2000 );
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
