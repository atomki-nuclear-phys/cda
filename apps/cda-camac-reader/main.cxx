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
#include <iostream>
#include <map>

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QtPlugin>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cmdl/cmdargs.h"
#   include "cdacore/device/Loader.h"
#   include "cdacore/camac/Crate.h"
#   include "cdacore/fifo/Fifo.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/BinaryStream.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "device/Loader.h"
#   include "camac/Crate.h"
#   include "fifo/Fifo.h"
#   include "event/Event.h"
#   include "event/BinaryStream.h"
#endif

// Local include(s):
#include "Crate.h"

// Function forward declaration(s):
void shutDown( int );

// Global variable(s):
msg::Logger   g_logger( "cda-camac-reader" );
camac::Crate* g_crate = 0;

// Description for the executable:
static const char* description =
   "Program reading events from the CAMAC crate for the CDA\n"
   "application suit.\n\n"
   "This executable should normally be started by CDA internally.\n"
   "You should only start it by hand for debugging purposes.";

// Load all the static plugins:
Q_IMPORT_PLUGIN( t2228a )

int main( int argc, char* argv[] ) {

   //
   // Read the command line options:
   //
   CmdArgInt verbosity( 'v', "verbosity", "code", "Level of output verbosity" );
   CmdArgStr config( 'c', "config", "filename", "Name of an XML config file",
                     CmdArg::isREQ );
   CmdArgStr server( 's', "server", "hostname", "Host name of the message server" );
   CmdArgInt port( 'p', "port", "number", "Port number of the message server" );

   CmdLine cmd( *argv, &verbosity, &config, &server, &port, NULL );
   cmd.description( description );

   CmdArgvIter arg_iter( --argc, ++argv );
   verbosity = 3;
   server = "127.0.0.1";
   port = 49700;
   cmd.parse( arg_iter );

   //
   // Set the destination of the messages:
   //
   msg::Sender::addAddress( msg::Address( ( const char* ) server, port ) );

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
   reader::Crate crate;
   crate.setLoader( &loader );
   if( ! crate.readConfig( doc.documentElement() ) ) {
      g_logger << msg::FATAL << "Failed to read configuration file!" << std::endl
               << "See previous messages for more information..." << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::INFO << "Read the configuration from: "
               << ( const char* ) config << msg::endmsg;
   }

   //
   // Open the CAMAC crate:
   //
   g_crate = new camac::Crate();
   if( ! g_crate->open() ) {
      g_logger << msg::FATAL << "Failed to open CAMAC crate" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::INFO << "Opened the CAMAC crate" << msg::endmsg;
   }

   // Reset the crate controller:
   g_crate->writeLong( 31, 0, 0, 0 );

   // Enable all CAMAC interrupts:
   //   g_crate->writeLong( 28, 1, 16, 0x00FFFFFF );

   //
   // Initialize the CAMAC devices for data acquisition:
   //
   if( ! crate.initialize( *g_crate ) ) {
      g_logger << msg::FATAL << "Failed to initialise devices for data "
               << "acquisition" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Initialised devices for data acquisition"
               << msg::endmsg;
   }

   //
   // Open the output FIFO:
   //
   QString fifo_path;
   fifo_path.append( CDASYS );
   fifo_path.append( "/fifos/histFifo" );
   Fifo fifo( fifo_path );
   if( ! fifo.open() ) {
      g_logger << msg::FATAL << "Couldn't open output FIFO" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Output FIFO successfully opened"
               << msg::endmsg;
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO << "CAMAC readout running..." << msg::endmsg;

   //
   // Read events and send them on the opened FIFO in an endless loop.
   //
   ev::BinaryStream evstream( &fifo );
   for( ; ; ) {

      ev::Event event = crate.readEvent( *g_crate );
      evstream << event;
      fifo.flush();

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
      g_logger << msg::ERROR << "Failed to close CAMAC crate" << msg::endmsg;
   } else {
      g_logger << msg::DEBUG << "Successfully closed CAMAC crate"
               << msg::endmsg;
   }
   delete g_crate;

   g_logger << msg::INFO << "Terminating application..." << msg::endmsg;
   exit( 0 );

   return;

}