// $Id$

// System include(s):
#include <cstdlib>
#include <csignal>

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdacore/i18n/Loader.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/common/Address.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/Sender.h"
#   include "cdadaq/stat/Sender.h"
#   include "cdadaq/config/ConfReader.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "i18n/Loader.h"
#   include "device/Loader.h"
#   include "common/Address.h"
#   include "event/Event.h"
#   include "event/Sender.h"
#   include "stat/Sender.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"
#include "../win32/plugins.h"

/// Function shutting down the data acquisition
void shutDown( int );

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
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStrList msgservers( 'm', "msgservers", "addresses",
                             "Addresses of message servers" );
   CmdArgStr config( 'c', "config", "filename/address",
                     "Name of an XML config file or address of a config server",
                     CmdArg::isREQ );
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
   // Decide how to read the configuration:
   //
   if( Address::isAddress( ( const char* ) config ) ) {

      //
      // Read the configuration data from the specified address:
      //
      conf::ConfReader reader;
      if( ! reader.readFrom( Address( ( const char* ) config ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-caen-reader",
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
                  << qApp->translate( "cda-caen-reader",
                                      "Couldn't process configuration "
                                      "coming from address: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-caen-reader",
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
                  << qApp->translate( "cda-caen-reader",
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
                  << qApp->translate( "cda-caen-reader",
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
                  << qApp->translate( "cda-caen-reader",
                                      "Successfully parsed: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

      //
      // Initialise a Crate object with this configuration:
      //
      QDomElement work = doc.documentElement();
      if( ! g_crate->readConfig( work ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-caen-reader",
                                      "Failed to read configuration file!\n"
                                      "See previous messages for more "
                                      "information..." )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-caen-reader",
                                      "Read the configuration from: %1" )
            .arg( ( const char* ) config )
                  << msg::endmsg;
      }

   }

   //
   // Initialize the digitizer(s) for data acquisition:
   //
   if( ! g_crate->initialize() ) {
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
   for( unsigned int i = 0; i < clients.count(); ++i ) {
      if( ! ev_sender.addSocket( Address( ( const char* ) clients[ i ] ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-caen-reader",
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
   cdastat::Sender stat_sender;
   for( unsigned int i = 0; i < statistics.count(); ++i ) {
      stat_sender.addReceiver( Address( ( const char* ) statistics[ i ] ) );
   }

   //
   // Construct the source string of the statistics objects that are sent out
   // during event processing:
   //
   QString statSource = "cda-caen-reader:";
   statSource += ( const char* ) config;
   statSource += ":";
   statSource += QString::number( QCoreApplication::applicationPid() );

   // Initialise the statistics information to something meaningful, then start
   // the statistics sender object:
   stat_sender.update( cdastat::Statistics( 0, statSource ) );
   stat_sender.start();

   //
   // Start the data acquisition:
   //
   if( ! g_crate->start() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't start the data acquisition" )
               << msg::endmsg;
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );
   signal( SIGTERM, shutDown );
   sigset_t blockedSignals;
   sigfillset( &blockedSignals );
   sigaddset( &blockedSignals, SIGINT );
   sigaddset( &blockedSignals, SIGTERM );

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
      sigprocmask( SIG_BLOCK, &blockedSignals, NULL );
      if( ! ev_sender.send( event ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-caen-reader",
                                      "Failed to send event to all recepients.\n"
                                      "Event readout can not continue!" )
                  << msg::endmsg;
         shutDown( 0 );
      }
      sigprocmask( SIG_UNBLOCK, &blockedSignals, NULL );

      // Update the statistics information after 10 events were sent out:
      ++g_evcount;
      if( ! ( g_evcount % 10 ) ) {
         stat_sender.update( cdastat::Statistics( g_evcount, statSource ) );
      }
   }

   return 0;
}

void shutDown( int ) {

   g_logger << msg::DEBUG
            << qApp->translate( "cda-caen-reader",
                                "Starting shutdown procedure" )
            << msg::endmsg;

   // Stop the data acquisition:
   if( ! g_crate->stop() ) {
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
   if( ! g_crate->finalize() ) {
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
