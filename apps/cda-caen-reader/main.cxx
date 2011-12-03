// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdacore/i18n/Loader.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/caen/Digitizer.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "i18n/Loader.h"
#   include "device/Loader.h"
#   include "caen/Digitizer.h"
#endif

// Global variable(s):
static msg::Logger g_logger( "cda-caen-reader" );

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

   CmdLine cmd( *argv, &verbosity, &msgservers, NULL );
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
   // Open the connection to the digitizer:
   //
   caen::Digitizer digitizer;
   if( ! digitizer.open() ) {
      g_logger << msg::FATAL
               << qApp->translate( "cda-caen-reader",
                                   "Couldn't open connection to digitizer" )
               << msg::endmsg;
      return 1;
   }

   return 0;
}
