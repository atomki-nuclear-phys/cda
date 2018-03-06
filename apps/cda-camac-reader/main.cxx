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
#include <map>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QCoreApplication>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#include "CDAVersion.h"
#include "msg/Sender.h"
#include "msg/Logger.h"
#include "tclap/CmdLine.h"
#include "device/Loader.h"
#include "camac/Crate.h"
#include "event/Event.h"
#include "event/Sender.h"
#include "i18n/Loader.h"
#include "stat/Sender.h"
#include "config/ConfReader.h"

// Local include(s):
#include "Crate.h"
#include "../win32/plugins.h"

// Function forward declaration(s):
void shutDown( int );
// Global variable(s):
static msg::Logger   g_logger( "cda-camac-reader" );
static camac::Crate* g_crate = 0;

// Description for the executable:
static const char* description =
   "Program reading events from a CAMAC crate for the CDA "
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
   auto itr = v_map.find( verbosity.getValue() );
   if( itr != v_map.end() ) {
      msg::Sender::instance()->setMinLevel( itr->second );
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
   if( dev::Loader::instance()->loadAll() ) {
      g_logger << msg::INFO
               << qApp->translate( "cda-camac-reader",
                                   "Successfully loaded all available devices" )
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL
               << qApp->translate( "cda-camac-reader",
                                   "There was an error loading the devices" )
               << msg::endmsg;
      return 1;
   }

   //
   // Create the crate object:
   //
   reader::Crate crate;
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
                  << qApp->translate( "cda-camac-reader",
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
                  << qApp->translate( "cda-camac-reader",
                                      "Couldn't process configuration "
                                      "coming from address: %1" )
            .arg( config.getValue().c_str() )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::INFO
                  << qApp->translate( "cda-camac-reader",
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
                  << qApp->translate( "cda-camac-reader",
                                      "The specified configuration file (\"%1\")\n"
                                      "could not be opened!" )
            .arg( config.getValue().c_str() ? config.getValue().c_str() : "" )
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
            .arg( config.getValue().c_str() ).arg( errorMsg )
            .arg( errorLine ).arg( errorColumn )
                  << msg::endmsg;
         return 1;
      } else {
         g_logger << msg::DEBUG
                  << qApp->translate( "cda-camac-reader",
                                      "Successfully parsed: %1" )
            .arg( config.getValue().c_str() )
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
            .arg( config.getValue().c_str() )
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
   for( const std::string& client : clients.getValue() ) {
      if( ! ev_sender.addSocket( Address( client.c_str() ) ) ) {
         g_logger << msg::FATAL
                  << qApp->translate( "cda-camac-reader",
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
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );
   signal( SIGTERM, shutDown );

   //
   // Construct the source string of the statistics objects that are sent out
   // during event processing:
   //
   QString statSource = "cda-camac-reader:";
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
         stat_sender.update( cdastat::Statistics( g_evcount, statSource ) );
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
