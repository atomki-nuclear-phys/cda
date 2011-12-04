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
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/common/Address.h"
#   include "cdadaq/config/ConfReader.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "i18n/Loader.h"
#   include "device/Loader.h"
#   include "caen/Digitizer.h"
#   include "common/Address.h"
#   include "config/ConfReader.h"
#endif

// Local include(s):
#include "Crate.h"

/// Function shutting down the data acquisition
void shutDown( int );

// Global variable(s):
static msg::Logger g_logger( "cda-caen-reader" );
static caen::Digitizer* g_dgtz = 0;

/// Description for the executable
static const char* description =
   "Program reading events from a CAEN digitizer for the CDA "
   "application suite.";

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

   CmdLine cmd( *argv, &verbosity, &config, &msgservers, NULL );
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
   dev::Loader loader;
   if( loader.loadAll() ) {
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
   caen_reader::Crate crate;
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
      if( ! crate.readConfig( reader.buffer() ) ) {
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
      if( ! crate.readConfig( work ) ) {
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
   // Open the connection to the digitizer:
   //
   g_dgtz = new caen::Digitizer();
   if( ! g_dgtz->open( crate.getConnType() ) ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't open connection to digitizer" )
               << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::INFO
               << qApp->translate( "cda-caen-reader",
                                   "Connection opened to the digitizer" )
               << msg::endmsg;
   }

   //
   // Initialize the digitizer(s) for data acquisition:
   //
   if( ! crate.initialize( *g_dgtz ) ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Failed to initialise devices for data "
                                   "acquisition" )
               << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-caen-reader",
                                   "Initialised devices for data acquisition" )
               << msg::endmsg;
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO
            << qApp->translate( "cda-caen-reader",
                                "CAEN readout running..." )
            << msg::endmsg;

   for( ; ; ) {

      ev::Event event = crate.readEvent( *g_dgtz );
   }

   return 0;
}

void shutDown( int ) {

   if( ! g_dgtz->close() ) {
      g_logger << msg::ERROR
               << qApp->translate( "cda-caen-reader",
                                   "Failed to close CAEN digitizer" )
               << msg::endmsg;
   } else {
      g_logger << msg::DEBUG
               << qApp->translate( "cda-caen-reader",
                                   "Successfully closed CAEN digitizer" )
               << msg::endmsg;
   }
   delete g_dgtz;

   g_logger << msg::INFO
            << qApp->translate( "cda-caen-reader",
                                "Terminating application..." )
            << msg::endmsg;
   exit( 0 );

   return;
}
