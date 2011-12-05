// $Id$

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdadaq/config/ConfServer.h"
#else
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "config/ConfServer.h"
#endif

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
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStr config( 'c', "config", "filename",
                     "Name of an XML or binary config file",
                     CmdArg::isREQ );
   CmdArgStrList msgservers( 'm', "msgservers", "addresses",
                             "Addresses of message servers" );
   CmdArgStr saddr( 's', "server-address", "address",
                    "Broadcast address of the configuration server" );

   CmdLine cmd( *argv, &verbosity, &config, &saddr, &msgservers, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   saddr = "127.0.0.1:31000";
   cmd.parse( arg_iter );

   //
   // Set the destination of the messages:
   //
   for( unsigned int i = 0; i < msgservers.count(); ++i ) {
      if( ! Address::isAddress( ( const char* ) msgservers[ i ] ) ) {
         continue;
      }
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
   conf::ConfServer server( ( const char* ) config );
   if( ! server.listen( Address( ( const char* ) saddr ) ) ) {
      logger << msg::FATAL
             << qApp->translate( "cda-config-server",
                                 "The configuration server could not be started "
                                 "on address: %1" ).arg( ( const char* ) saddr )
             << msg::endmsg;
      return 1;
   } else {
      logger << msg::INFO
             << qApp->translate( "cda-config-server",
                                 "The configuration server is broadcasting \"%1\" "
                                 "on address \"%2\"" )
         .arg( ( const char* ) config ).arg( ( const char* ) saddr )
             << msg::endmsg;
   }

   return app.exec();
}
