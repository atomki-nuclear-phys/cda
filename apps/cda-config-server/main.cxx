// $Id$

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "CDAVersion.h"
#include "msg/Logger.h"
#include "tclap/CmdLine.h"
#include "config/ConfServer.h"

// Local include(s):
#include "../win32/plugins.h"

/// Description printed to the console
static const char* description =
   "This program can be used to broadcast a configuration to other "
   "members of the CDA application suite through the network.";

int main( int argc, char* argv[] ) {

   // Create the message logging object:
   msg::Logger logger( "cda-config-server" );

   QCoreApplication app( argc, argv );

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
   TCLAP::ValueArg< std::string >
         saddr( "s", "server-address",
                "Broadcast address of the configuration server", false,
                "127.0.0.1:31000", "address" );
   cmd.parse( argc, argv );

   //
   // Set the destination of the messages:
   //
   for( const std::string& address : msgservers.getValue() ) {
      if( ! Address::isAddress( address.c_str() ) ) {
         continue;
      }
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
      logger << msg::FATAL
             << qApp->translate( "cda-config-server",
                                 "Didn't recognise verbosity level setting\n"
                                 "Terminating..." )
             << msg::endmsg;
      return 1;
   }

   //
   // Start the configuration server on a specific port:
   //
   conf::ConfServer server( config.getValue().c_str() );
   if( ! server.listen( Address( saddr.getValue().c_str() ) ) ) {
      logger << msg::FATAL
             << qApp->translate( "cda-config-server",
                                 "The configuration server could not be "
                                 "started on address: %1" )
                .arg( saddr.getValue().c_str() )
             << msg::endmsg;
      return 1;
   } else {
      logger << msg::INFO
             << qApp->translate( "cda-config-server",
                                 "The configuration server is broadcasting "
                                 "\"%1\" on address \"%2\"" )
                .arg( config.getValue().c_str() )
                .arg( saddr.getValue().c_str() )
             << msg::endmsg;
   }

   return app.exec();
}
