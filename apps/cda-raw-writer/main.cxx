// $Id$
/**
 *   @file apps/cda-raw-writer/main.cxx
 *  @short Main file for the RAW writer application
 *
 *         This file stores the code that runs the cda-raw-writer application.
 *         This application is used to permanently store the data coming from
 *         the DAQ system in an application specific RAW format.
 *
 * @author Attila Krasznahorkay Jr. <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */

// System include(s):
#include <cstdlib>
#include <csignal>
#include <unistd.h>

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
#   include "event/Event.h"
#   include "event/EventServer.h"
#   include "i18n/Loader.h"
#   include "stat/Sender.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"
#include "FileWriter.h"

// Function forward declaration(s):
void shutDown( int );

// Global variable(s):
msg::Logger   g_logger( "cda-raw-writer" );
raw::Crate* g_crate = 0;
raw::FileWriter* g_fwriter = 0;
quint32 g_evcount = 0;

/// Description printed to the console
static const char* description =
   "Program writing events which it receives, to a RAW format file. "
   "The output file contains the events in the binary format that is "
   "used for sending the data between the applications.\n\n"
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
   CmdArgStr evaddress( 'e', "evaddress", "address",
                        "Address where to receive events",
                        CmdArg::isREQ );
   CmdArgStr output( 'o', "output", "filename", "Name of the RAW file",
                     CmdArg::isREQ );
   CmdArgStrList statistics( 's', "statistics", "addresses",
                             "Addresses of statistics reader clients" );
   CmdArgInt updatefreq( 'u', "update-frequency", "minutes",
                         "Frequency of file name updates" );

   CmdLine cmd( *argv, &verbosity, &config, &msgservers, &evaddress, &output,
                &statistics, &updatefreq, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   updatefreq = 60;
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
               << qApp->translate( "cda-raw-writer",
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
               << qApp->translate( "cda-raw-writer",
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
               << qApp->translate( "cda-raw-writer",
                                   "Successfully loaded all available devices" )
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL
               << qApp->translate( "cda-raw-writer",
                                   "There was an error loading the devices" )
               << msg::endmsg;
   }

   //
   // Create the crate object:
   //
   g_crate = new raw::Crate();
   g_crate->setLoader( &loader );

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
                  << qApp->translate( "cda-raw-writer",
                                      "Couldn't read configuration from "
                                      "address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      }

      //
      // Initialise the crate object from the buffer:
      //
      if( ! g_crate->readConfig( reader.buffer() ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-raw-writer",
                                      "Couldn't process configuration coming "
                                      "from address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-raw-writer",
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
                  << qApp->translate( "cda-raw-writer",
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
                  << qApp->translate( "cda-raw-writer",
                                      "Error in parsing \"%1\"\n"
                                      "  Error message: %2\n"
                                      "  Error line   : %3\n"
                                      "  Error column : %4" )
            .arg( ( const char* ) config )
            .arg( errorMsg ).arg( errorLine ).arg( errorColumn )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate( "cda-raw-writer",
                                      "Successfully parsed: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

      QDomElement work = doc.documentElement();

      //
      // Initialise a Crate object with this configuration:
      //
      if( ! g_crate->readConfig( work ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-raw-writer",
                                      "Failed to read configuration file!\n"
                                      "See previous messages for more "
                                      "information..." )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-raw-writer",
                                      "Read the configuration from: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

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

   //
   // Construct the source string of the statistics objects that are sent out
   // during event processing:
   //
   QString statSource = "cda-raw-writer:";
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
            << qApp->translate( "cda-raw-writer",
                                "RAW writing running..." )
            << msg::endmsg;

   //
   // Create the thread that will take care of writing the output file(s):
   //
   g_fwriter = new raw::FileWriter( evserver, *g_crate );

   //
   // Decide if file name updating will be required:
   //
   QString fileName( ( const char* ) output );
   if( fileName.contains( "%1" ) ) {
      //
      // Initialise writing to the first RAW file:
      //
      int filecounter = 1;
      if( ! g_crate->initialize( fileName.arg( filecounter, ( int ) 3, ( int ) 10,
                                               QLatin1Char( '0' ) ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-raw-writer",
                                      "Failed to initialise RAW file for data "
                                      "acquisition" )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate( "cda-raw-writer",
                                      "Initialised RAW file for data acquisition" )
                  << msg::endmsg;
      }
      //
      // Start the file writing and monitoring (this) threads:
      //
      g_fwriter->start();
      QTime startTime( QTime::currentTime() );
      for( ; ; ) {

         // Check if the file writing thread is still running:
         if( ! g_fwriter->isRunning() ) {
            g_logger << msg::FATAL
                     << qApp->translate( "cda-raw-writer",
                                         "The file writing thread "
                                         "unexpectedly died" )
                     << msg::endmsg;
            shutDown( 0 );
         }

         // Update the statistics receivers:
         stat_sender.update( cdastat::Statistics( g_evcount +
                                               g_fwriter->processedEvents(),
                                               statSource ) );

         // If it's time to open a new file, let's do it:
         if( startTime.secsTo( QTime::currentTime() ) > ( 60 * updatefreq ) ) {
            // Remember the current time:
            startTime = QTime::currentTime();
            // First off, let's stop the file writing thread:
            g_fwriter->stopProcessing();
            // Store how much data we saved to this file:
            g_evcount += g_fwriter->processedEvents();
            // Now close the current output file:
            g_crate->finalize();
            // Open a new file:
            ++filecounter;
            if( ! g_crate->initialize( fileName.arg( filecounter, ( int ) 3,
                                                     ( int ) 10,
                                                     QLatin1Char( '0' ) ) ) ) {
               g_logger << msg::FATAL
                        << qApp->translate( "cda-raw-writer",
                                            "Failed to initialise RAW file "
                                            "for data acquisition" )
                        << msg::endmsg;
               return 1;
            } else {
               g_logger << msg::DEBUG
                        << qApp->translate( "cda-raw-writer",
                                            "Initialised RAW file for data "
                                            "acquisition" )
                        << msg::endmsg;
            }
            // Start the file writing thread once more:
            g_fwriter->start();
         }

         // Sleep for 2 seconds:
#ifdef Q_WS_WIN32
         _sleep( 2 );
#else
         sleep( 2 );
#endif // Q_WS_WIN
      }
   } else {
      //
      // Initialise writing to the RAW file:
      //
      if( ! g_crate->initialize( fileName ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-raw-writer",
                                      "Failed to initialise RAW file for data "
                                      "acquisition" )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate( "cda-raw-writer",
                                      "Initialised RAW file for data acquisition" )
                  << msg::endmsg;
      }
      //
      // Start the file writing and monitoring (this) threads:
      //
      g_fwriter->start();
      for( ; ; ) {

         // Check if the file writing thread is still running:
         if( ! g_fwriter->isRunning() ) {
            g_logger << msg::FATAL
                     << qApp->translate( "cda-raw-writer",
                                         "The file writing thread unexpectedly "
                                         "died" )
                     << msg::endmsg;
            shutDown( 0 );
         }

         // Update the statistics receivers:
         stat_sender.update( cdastat::Statistics( g_fwriter->processedEvents(),
                                               statSource ) );

         // Sleep for 2 seconds:
#ifdef Q_WS_WIN32
         _sleep( 2 );
#else
         sleep( 2 );
#endif // Q_WS_WIN
      }
   }

   //
   // The code is never supposed to reach this statement, but it's nicer
   // to have the compiler satisfied...
   //
   return 0;
}

/**
 * This function makes sure that the output RAW file is properly closed
 * when the application terminates.
 */
void shutDown( int ) {

   // Stop the event processing thread:
   g_fwriter->stopProcessing();

   // Close the current output file:
   g_crate->finalize();

   g_logger << msg::INFO
            << qApp->translate( "cda-raw-writer",
                                "Total number of events processed: %1" )
      .arg( g_evcount + g_fwriter->processedEvents() )
            << msg::endmsg;
   g_logger << msg::INFO
            << qApp->translate( "cda-raw-writer",
                                "Terminating application..." )
            << msg::endmsg;

   // Clean up after ourselves:
   delete g_fwriter;
   delete g_crate;

   exit( 0 );

   return;
}
