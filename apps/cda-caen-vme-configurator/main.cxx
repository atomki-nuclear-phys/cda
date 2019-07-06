
// System include(s).
#include <cstdlib>
#include <csignal>

// STL include(s).
#include <map>

// Qt include(s).
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s).
#include "CDAVersion.h"
#include "msg/Sender.h"
#include "msg/Logger.h"
#include "tclap/CmdLine.h"
#include "i18n/Loader.h"
#include "device/Loader.h"
#include "common/Address.h"
#include "config/ConfReader.h"

// Local include(s).
#include "Crate.h"
#include "../win32/plugins.h"

/// Description for the executable
static const char* DESCRIPTION =
   "Program configuring VME devices that do not take part in the data "
   "readout.\n"
   "It should be executed on the DAQ machine before starting the data "
   "acquisition, to set up all devices that sit in the VME crate, but "
   "do not provide readout data to the DAQ system.";

/// The name of the application
static const char* APP_NAME = "cda-caen-vme-configurator";

int main( int argc, char* argv[] ) {

   StatusCode::enableFailure();

   // Read the command line options.
   TCLAP::CmdLine cmd( DESCRIPTION, ' ', CDA_VERSION_STRING );
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
   cmd.parse( argc, argv );

   // Set the destination of the messages.
   for( const std::string& address : msgservers.getValue() ) {
      msg::Sender::addAddress( Address( address.c_str() ) );
   }

   // Set up a message logger object.
   msg::Logger logger( APP_NAME );

   // Translate the verbosity option.
   static const std::map< int, msg::Level > v_map {
      { 1, msg::VERBOSE },
      { 2, msg::DEBUG },
      { 3, msg::INFO },
      { 4, msg::WARNING },
      { 5, msg::ERROR },
      { 6, msg::FATAL },
      { 7, msg::ALWAYS }
   };
   auto itr = v_map.find( verbosity.getValue() );
   if( itr != v_map.end() ) {
      msg::Sender::instance()->setMinLevel( itr->second );
   } else {
      logger << msg::FATAL
             << qApp->translate( APP_NAME,
                                 "Didn't recognise verbosity level setting\n"
                                 "Terminating..." )
             << msg::endmsg;
      return 1;
   }

   // Load all the available translations.
   QCoreApplication app( argc, argv );
   i18n::Loader trans_loader;
   if( ! trans_loader.loadTranslations() ) {
      logger << msg::FATAL
             << qApp->translate( APP_NAME,
                                 "Couldn't load the translations!" )
             << msg::endmsg;
      return 1;
   }

   // Try to load all available device plugins.
   if( dev::Loader::instance()->loadAll() ) {
      logger << msg::INFO
             << qApp->translate( APP_NAME,
                                 "Successfully loaded all available devices" )
             << msg::endmsg;
   } else {
      logger << msg::FATAL
             << qApp->translate( APP_NAME,
                                 "There was an error loading the devices" )
             << msg::endmsg;
      return 1;
   }

   // Create the crate object.
   caen_vme_configurator::Crate crate;
   crate.setLoader( dev::Loader::instance() );

   // Decide how to read the configuration.
   if( Address::isAddress( config.getValue().c_str() ) ) {

      // Read the configuration data from the specified address.
      conf::ConfReader reader;
      if( ! reader.readFrom( Address( config.getValue().c_str() ) ) ) {
         logger << msg::FATAL
                << qApp->translate( APP_NAME,
                                    "Couldn't read configuration from "
                                    "address: %1" )
                   .arg( config.getValue().c_str() )
                << msg::endmsg;
         return 1;
      }

      // Initialise the crate object from the buffer.
      if( ! crate.readConfig( reader.buffer() ).isSuccess() ) {
         logger << msg::FATAL
                << qApp->translate( APP_NAME,
                                    "Couldn't process configuration "
                                    "coming from address: %1" )
                   .arg( config.getValue().c_str() )
                << msg::endmsg;
         return 1;
      } else {
         logger << msg::INFO
                << qApp->translate( APP_NAME,
                                    "Read the configuration from: %1" )
                   .arg( config.getValue().c_str() )
                << msg::endmsg;
      }

   } else {

      // Open the configuration file.
      QFile config_file( config.getValue().c_str() );
      if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
         logger << msg::FATAL
                << qApp->translate( APP_NAME,
                                    "The specified configuration file "
                                    "(\"%1\") could not be opened!" )
                   .arg( config.getValue().c_str() )
                << msg::endmsg;
         return 1;
      }

      // Read the file's contents as an XML.
      QDomDocument doc;
      QString errorMsg;
      int errorLine, errorColumn;
      if( ! doc.setContent( &config_file, false, &errorMsg, &errorLine,
                            &errorColumn ) ) {
         logger << msg::FATAL
                << qApp->translate( APP_NAME,
                                    "Error in parsing \"%1\"\n"
                                    "  Error message: %2\n"
                                    "  Error line   : %3\n"
                                    "  Error column : %4" )
                   .arg( config.getValue().c_str() ).arg( errorMsg )
                   .arg( errorLine ).arg( errorColumn )
                << msg::endmsg;
         return 1;
      } else {
         logger << msg::DEBUG
                << qApp->translate( APP_NAME,
                                    "Successfully parsed: %1" )
                   .arg( config.getValue().c_str() )
                << msg::endmsg;
      }

      // Initialise a Crate object with this configuration.
      QDomElement work = doc.documentElement();
      if( ! crate.readConfig( work ).isSuccess() ) {
         logger << msg::FATAL
                << qApp->translate( APP_NAME,
                                    "Failed to read configuration file!\n"
                                    "See previous messages for more "
                                    "information..." )
                << msg::endmsg;
         return 1;
      } else {
         logger << msg::INFO
                << qApp->translate( APP_NAME,
                                    "Read the configuration from: %1" )
                   .arg( config.getValue().c_str() )
                << msg::endmsg;
      }

   }

   // Configure all the VME devices.
   if( ! crate.configure().isSuccess() ) {
      logger << msg::FATAL
             << qApp->translate( APP_NAME,
                                 "Failed to configure the device(s)" )
             << msg::endmsg;
      return 1;
   } else {
      logger << msg::INFO
             << qApp->translate( APP_NAME,
                                 "Finished configuring device(s)" )
             << msg::endmsg;
   }

   // Return gracefully.
   return 0;
}