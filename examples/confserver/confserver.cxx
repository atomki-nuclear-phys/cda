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

int main( int argc, char* argv[] ) {

   msg::Logger logger( "confserver" );

   QCoreApplication app( argc, argv );

   //
   // Read the command line options:
   //
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStr config( 'c', "config", "filename",
                     "Name of an XML or binary config file",
                     CmdArg::isREQ );
   CmdArgStrList msgservers( 'm', "msgservers",
                             "addresses", "Addresses of message servers" );

   CmdLine cmd( *argv, &verbosity, &config, &msgservers, NULL );

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
      logger << msg::FATAL << "Didn't recognise verbosity level setting"
             << std::endl
             << "Terminating..." << msg::endmsg;
      return 1;
   }

   //
   // Start the configuration server on a specific port:
   //
   conf::ConfServer server( ( const char* ) config );
   if( ! server.listen( Address( "0.0.0.0", 31000 ) ) ) {
      logger << msg::FATAL << "The configuration server could not be started" << msg::endmsg;
      return 1;
   } else {
      logger << msg::INFO << "The configuration server is broadcasting: "
             << ( const char* ) config << msg::endmsg;
   }

   return app.exec();

}
