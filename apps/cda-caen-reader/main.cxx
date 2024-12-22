
// System include(s):
#include <cstdlib>
#include <csignal>

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

// CDA include(s):
#include "CDAVersion.h"
#include "msg/Sender.h"
#include "msg/Logger.h"
#include "tclap/CmdLine.h"
#include "i18n/Loader.h"
#include "device/Loader.h"
#include "common/Address.h"
#include "event/Event.h"
#include "event/Sender.h"
#include "caen/StopAcquisition.h"
#include "stat/Sender.h"
#include "config/ConfReader.h"

// Local include(s):
#include "Crate.h"
#include "../common/plugins.h"
#include "../common/readConfig.h"

/// Function shutting down the data acquisition
void shutDown( int );
/// Function preparing the application for the shutdown
void prepareShutDown( int );

// Global variable(s):
static msg::Logger g_logger( "cda-caen-reader" );
static caen_reader::Crate* g_crate = 0;

/// Description for the executable
static const char* description =
   "Program reading events from a (set of) CAEN digitizer(s) for the CDA "
   "application suite.\n\n"
   "This executable should normally be started by CDA internally. "
   "You should only start it by hand for debugging purposes.";

/// Number of events processed
quint32 g_evcount = 0;

int main( int argc, char* argv[] ) {

   //
   // Read the command line options:
   //
   TCLAP::CmdLine cmd( description, ' ', CDA_VERSION_STRING );
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
   TCLAP::MultiArg< std::string >
           clients( "e", "clients", "Addresses of event reader clients",
                    true, "address list" );
   cmd.add( clients );
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
               << qApp->translate( "cda-caen-reader",
                                   "Didn't recognise verbosity level setting\n"
                                   "Terminating..." )
               << msg::endmsg;
      return 1;
   }

   //
   // Load all the available translations:
   //
   QCoreApplication app( argc, argv );
   i18n::Loader trans_loader;
   if( ! trans_loader.loadTranslations() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't load the translations!" )
               << msg::endmsg;
      return 1;
   }

   //
   // Try to load all available device plugins:
   //
   if( dev::Loader::instance()->loadAll() ) {
      g_logger << msg::INFO
               << qApp->translate( "cda-caen-reader",
                                   "Successfully loaded all available devices" )
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "There was an error loading the devices" )
               << msg::endmsg;
      return 1;
   }

   //
   // Create the crate object:
   //
   g_crate = new caen_reader::Crate();
   g_crate->setLoader( dev::Loader::instance() );

   //
   // Read the configuration:
   //
   if(apps::readConfig(config.getValue().c_str(), *g_crate).isFailure()) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't read the configuration!" )
               << msg::endmsg;
      return 1;
   }

   //
   // Initialize the digitizer(s) for data acquisition:
   //
   if( ! g_crate->initialize().isSuccess() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Failed to initialise device(s) for data "
                                   "acquisition" )
               << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-caen-reader",
                                   "Initialised device(s) for data acquisition" )
               << msg::endmsg;
   }

   //
   // Open connections to all the event recepients:
   //
   ev::Sender ev_sender;
   for( const std::string& client : clients.getValue() ) {
      if( ! ev_sender.addSocket( Address( client.c_str() ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-caen-reader",
                                      "Couldn't connect to event receiver: %1" )
            .arg( client.c_str() )
                  << msg::endmsg;
         shutDown( 0 );
      }
   }

   //
   // Open connections to all the statistics recepients. (Ignore connection errors
   // here, since statistics publishing is not a major concern...)
   //
   cdastat::Sender stat_sender;
   for( const std::string& server : statistics.getValue() ) {
      stat_sender.addReceiver( Address( server.c_str() ) );
   }

   //
   // Construct the source string of the statistics objects that are sent out
   // during event processing:
   //
   QString statSource = "cda-caen-reader:";
   statSource += config.getValue().c_str();
   statSource += ":";
   statSource += QString::number( QCoreApplication::applicationPid() );

   // Initialise the statistics information to something meaningful, then start
   // the statistics sender object:
   stat_sender.update( cdastat::Statistics( 0, statSource ) );
   stat_sender.start();

   //
   // Start the data acquisition:
   //
   if( ! g_crate->start().isSuccess() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't start the data acquisition" )
               << msg::endmsg;
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, prepareShutDown );
   signal( SIGTERM, prepareShutDown );
#ifndef Q_OS_WIN32
   sigset_t blockedSignals;
   sigfillset( &blockedSignals );
   sigaddset( &blockedSignals, SIGINT );
   sigaddset( &blockedSignals, SIGTERM );
#endif // Q_OS_WIN32

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO
            << qApp->translate( "cda-caen-reader",
                                "CAEN readout running..." )
            << msg::endmsg;

   //
   // Read the events, and send them out in an endless loop:
   //
   g_evcount = 0;
   for( ; ; ) {

      // Read and send an event:
      const ev::Event event = g_crate->readEvent();

      // Exit data acquisition if there was an interrupt:
      if( g_stopAcquisition ) break;

      // Send the event to the receivers:
#ifndef Q_OS_WIN32
      sigprocmask( SIG_BLOCK, &blockedSignals, NULL );
#endif // Q_OS_WIN32
      if( ! ev_sender.send( event ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-caen-reader",
                                      "Failed to send event to all recepients.\n"
                                      "Event readout can not continue!" )
                  << msg::endmsg;
         shutDown( 0 );
      }
#ifndef Q_OS_WIN32
      sigprocmask( SIG_UNBLOCK, &blockedSignals, NULL );
#endif // Q_OS_WIN32

      // Update the statistics information after 10 events were sent out:
      ++g_evcount;
      if( ! ( g_evcount % 10 ) ) {
         stat_sender.update( cdastat::Statistics( g_evcount, statSource ) );
      }
   }

   // Shut down cleanly:
   shutDown( 0 );

   return 0;
}

/**
 * This function is used to clean up after the application, once the user
 * sends an interrupt to it.
 */
void shutDown( int ) {

   g_logger << msg::DEBUG
            << qApp->translate( "cda-caen-reader",
                                "Starting shutdown procedure" )
            << msg::endmsg;

   // Stop the data acquisition:
   if( ! g_crate->stop().isSuccess() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't stop data acquisition" )
               << msg::endmsg;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-caen-reader",
                                   "Data acquisition stopped" )
               << msg::endmsg;
   }
   // Finalize the data acquisition:
   if( ! g_crate->finalize().isSuccess() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't cleanly finalize data acquisition" )
               << msg::endmsg;
   } else {
      g_logger << msg::INFO
               << qApp->translate( "cda-caen-reader",
                                   "Data acquisition finalized" )
               << msg::endmsg;
   }
   delete g_crate;

   g_logger << msg::INFO
            << qApp->translate( "cda-caen-reader",
                                "Total number of events read: %1" )
      .arg( g_evcount )
            << msg::endmsg;
   g_logger << msg::INFO
            << qApp->translate( "cda-caen-reader",
                                "Terminating application..." )
            << msg::endmsg;
   exit( 0 );

   return;
}

/**
 * Modifies the global variable <code>g_stopAcquisition</code> to tell all
 * parts of this application that the application should stop as soon as
 * possible.
 */
void prepareShutDown( int ) {

   g_stopAcquisition = true;
   return;
}
