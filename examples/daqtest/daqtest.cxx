// $Id$

// System include(s):
extern "C" {
#   include <unistd.h>
}

// CDA include(s):
#include <QtCore/QtGlobal>
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdacore/msg/Sender.h"
#   include "cdadaq/common/AppRunner.h"
#   include "cdadaq/common/PathResolver.h"
#else
#   include "msg/Logger.h"
#   include "msg/Sender.h"
#   include "common/AppRunner.h"
#   include "common/PathResolver.h"
#endif

static msg::Logger g_logger( "daqtest" );

int main() {

   msg::Sender::addAddress( Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::VERBOSE );

   QString exec_path = daq::PathResolver::resolve( "echo", "PATH" );
   exec_path = daq::PathResolver::resolve( "echo", "PATH" );
   if( exec_path == "" ) {
      g_logger << msg::FATAL << "Couldn't find location of \"echo\" executable"
               << msg::endmsg;
      return 255;
   }
   daq::AppRunner runner( exec_path, "asc cda erf m test" );

   if( ! runner.start() ) {
      g_logger << msg::FATAL << "Unable to start executable!" << msg::endmsg;
      return 255;
   } else {
      g_logger << msg::INFO << "Started executable" << msg::endmsg;
   }

   sleep( 2 );

   runner.stop();

   exec_path = daq::PathResolver::resolve( "cda-glomem-writer", "PATH" );
   daq::AppRunner cdaRunner( exec_path, "-m 127.0.0.1:49700 "
                             "-c /home/krasznaa/CDA/configs/t2228a_config.cxml "
                             "-e 127.0.0.1:50000" );
   if( ! cdaRunner.start() ) {
      g_logger << msg::FATAL << "Unable to start CDA executable!" << msg::endmsg;
      return 255;
   } else {
      g_logger << msg::INFO << "CDA executable started" << msg::endmsg;
   }

   sleep( 2 );
   cdaRunner.stop();

   return 0;

}
