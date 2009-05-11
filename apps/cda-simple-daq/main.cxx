// $Id$

// Qt include(s):
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdacore/msg/Sender.h"
#   include "cdacore/cmdl/cmdargs.h"
#else
#   include "msg/Logger.h"
#   include "msg/Sender.h"
#   include "cmdl/cmdargs.h"
#endif

// Local include(s):
#include "SimpleDAQWindow.h"
#include "Constants.h"

static const char* description =
   "Main DAQ program for running CDA on a single machine that does\n"
   "all the work. (Read from the CAMAC crate, write to the PAW global\n"
   "memory and write to an HBOOK file.)";

msg::Logger g_logger( "cda-simple-daq" );

int main( int argc, char* argv[] ) {

   //
   // Create a Qt application and let it remove the Qt-related entries from the
   // command line arguments:
   //
   QApplication app( argc, argv );

   //
   // Read the rest of the command line arguments:
   //
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStr config( 'c', "config", "filename", "Name of the XML configuration",
                     CmdArg::isREQ );

   CmdLine cmd( *argv, &verbosity, &config, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   cmd.parse( arg_iter );

   // Set the message server to a fixed address:
   msg::Sender::addAddress( Address( Const::MSG_SERVER_ADDRESS ) );

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
      g_logger << msg::FATAL << "Didn't recognise verbosity level setting"
               << std::endl
               << "Terminating..." << msg::endmsg;
      return 1;
   }

   SimpleDAQWindow window( ( const char* ) config,
                           v_map.find( verbosity )->second );
   window.show();

   return app.exec();

}
