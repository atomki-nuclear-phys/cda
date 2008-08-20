// $Id$
/**
 *   @file apps/cda-hbook-writer/main.cxx
 *  @short Main file for the HBOOK writer application
 *
 *         This file stores the code that runs the cda-hbook-writer application.
 *         This application is used to permanently store the data coming from
 *         the DAQ system.
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
#include <iostream>
#include <map>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/EventServer.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "device/Loader.h"
#   include "event/Event.h"
#   include "event/EventServer.h"
#endif

// Local include(s):
#include "Crate.h"

// Function forward declaration(s):
void shutDown( int );

// Global variable(s):
msg::Logger   g_logger( "cda-hbook-writer" );
hbook::Crate* g_crate = 0;

static const char* description =
   "Program writing events which it receives, to a HBOOK format file.\n"
   "The output file contains one ntuple with all the variables defined\n"
   "in the data acquisition setup.\n\n"
   "This executable should normally be started by CDA internally.\n"
   "You should only start it by hand for debugging purposes.";

int main( int argc, char* argv[] ) {

   //
   // Read the command line options:
   //
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStr config( 'c', "config", "filename", "Name of an XML config file",
                     CmdArg::isREQ );
   CmdArgStr server( 's', "server", "hostname", "Host name of the message server" );
   CmdArgInt port( 'p', "port", "number", "Port number of the message server" );
   CmdArgInt fifofd( 'f', "fifofd", "number", "Use this file descriptor(fifo) insted of tcp listen" );
   CmdArgStr output( 'o', "output", "filename", "Name of HBOOK file",
                     CmdArg::isREQ );

   CmdLine cmd( *argv, &verbosity, &config, &server, &port, &fifofd, &output, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   server = "127.0.0.1";
   port = 49700;
   fifofd=-1;
   cmd.parse( arg_iter );

   //
   // Set the destination of the messages:
   //
   msg::Sender::addAddress( Address( ( const char* ) server, port ) );

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

   //
   // Open the configuration file:
   //
   QFile config_file( ( const char* ) config );
   if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
      g_logger << msg::FATAL << "The specified configuration file (\""
               << ( ( const char* ) config ? ( const char* ) config : "" )
               << "\")" << std::endl
               << "could not be opened!" << msg::endmsg;
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
      g_logger << msg::FATAL << "Error in parsing \"" << ( const char* ) config
               << "\"" << std::endl
               << "  Error message: " << errorMsg << std::endl
               << "  Error line   : " << errorLine << std::endl
               << "  Error column : " << errorColumn << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Successfully parsed: "
               << ( const char* ) config << msg::endmsg;
   }

   //
   // Try to load all available device plugins:
   //
   dev::Loader loader;
   if( loader.loadAll() ) {
      g_logger << msg::INFO << "Successfully loaded all available devices"
               << msg::endmsg;
   } else {
      g_logger << msg::FATAL << "There was an error loading the devices"
               << msg::endmsg;
   }

   //
   // Initialise a Crate object with this configuration:
   //
   g_crate = new hbook::Crate();
   g_crate->setLoader( &loader );
   if( ! g_crate->readConfig( doc.documentElement() ) ) {
      g_logger << msg::FATAL << "Failed to read configuration file!" << std::endl
               << "See previous messages for more information..." << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::INFO << "Read the configuration from: "
               << ( const char* ) config << msg::endmsg;
   }

   //
   // Initialise writing to a HBOOK file:
   //
   if( ! g_crate->initialize( ( const char* ) output ) ) {
      g_logger << msg::FATAL << "Failed to initialise HBOOK file for data "
               << "acquisition" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Initialised HBOOK file for data acquisition"
               << msg::endmsg;
   }

   //
   // Start an EventServer listening on "a" port:
   //
   ev::EventServer evserver;
   evserver.listen( Address( "127.0.0.1", 45100 ) );

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO << "HBOOK writing running..." << msg::endmsg;

   //
   // Read events and give them to the crate to write, in an endless loop.
   //
   for( ; ; ) {

      ev::Event event;
      evserver >> event;

      if( ! g_crate->writeEvent( event ) ) {

         g_logger << msg::FATAL << "There was a problem writing an event"
                  << msg::endmsg;
         return 1;

      }

   }

   //
   // The code is never supposed to reach this statement, but it's nicer
   // to have the compiler satisfied...
   //
   return 0;

}

/**
 * This function makes sure that the output HBOOK file is properly closed
 * when the application terminates.
 */
void shutDown( int ) {

   g_crate->finalize();
   delete g_crate;

   g_logger << msg::INFO << "Terminating application..." << msg::endmsg;
   exit( 0 );

   return;

}
