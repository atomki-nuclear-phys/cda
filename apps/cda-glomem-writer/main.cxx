// $Id$
/**
 *   @file main.cxx
 *  @short Main file for the monitoring histogram application
 *
 *         This file stores the code that runs the cda-glomem-writer application.
 *         This application is used to fill monitoring histograms with the
 *         data read from the CAMAC devices.
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
#   include "cdacore/fifo/Fifo.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/BinaryStream.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "cmdl/cmdargs.h"
#   include "device/Loader.h"
#   include "fifo/Fifo.h"
#   include "event/Event.h"
#   include "event/BinaryStream.h"
#endif

// Local include(s):
#include "Crate.h"

// Function forward declaration(s):
void shutDown( int );

// Global variable(s):
msg::Logger g_logger( "cda-glomem-writer" );

// Description for the executable:
static const char* description =
   "Program writing events which it receives, to a global memory\n"
   "block of PAW. This makes it possible to monitor the data-taking\n"
   "from within PAW++ while the data acquisition is on-going.\n\n"
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
   Crate crate;
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
   // Initialise the monitoring histograms:
   //
   if( ! crate.initialize() ) {
      g_logger << msg::FATAL << "Failed to initialise histograms for data "
               << "acquisition" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Initialised histograms for data acquisition"
               << msg::endmsg;
   }

   //
   // Open the input FIFO:
   //
   QString fifo_path;
   fifo_path.append( CDASYS );
   fifo_path.append( "/fifos/histFifo" );
   Fifo fifo( fifo_path );
   if( ! fifo.open() ) {
      g_logger << msg::FATAL << "Couldn't open input FIFO" << msg::endmsg;
      return 1;
   } else {
      g_logger << msg::DEBUG << "Input FIFO successfully opened"
               << msg::endmsg;
   }

   //
   // Connect the interrupt signal to the shutDown function:
   //
   signal( SIGINT, shutDown );

   //
   // Let the user know what we're doing:
   //
   g_logger << msg::INFO << "Histogram writing running..." << msg::endmsg;

   //
   // Read events and give them to the crate to display, in an endless loop.
   //
   ev::BinaryStream evstream( &fifo );
   for( ; ; ) {

      ev::Event event;
      evstream >> event;

      if( ! crate.displayEvent( event ) ) {

         g_logger << msg::FATAL << "There was a problem diplaying an event"
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
 * In the case of this application having this shutdown function is a bit of
 * an overkill. But it's nice to have all the CDA applications behaving the
 * same way...
 */
void shutDown( int ) {

   g_logger << msg::INFO << "Terminating application..." << msg::endmsg;
   exit( 0 );

   return;

}
