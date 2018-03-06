// $Id$
/**
 *   @file apps/cda-camac-daq/main.cxx
 *  @short Main file for the cda-camac-daq application
 *
 *         This file stores the code that runs the cda-camac-daq application.
 *         It is rather simple, it reads a few simple command line options,
 *         then sets up CamacDAQWindow to do the rest of the job.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QApplication>

// CDA include(s):
#include "CDAVersion.h"
#include "msg/Logger.h"
#include "msg/Sender.h"
#include "tclap/CmdLine.h"
#include "i18n/Loader.h"
#include "common/DefaultFont.h"
#include "common/SplashScreen.h"

// Local include(s):
#include "CamacDAQWindow.h"
#include "Constants.h"

// Description for the executable:
static const char* description =
   "Main DAQ program for running CDA on a single machine that does\n"
   "all the work. (Read from the CAMAC crate, write to the PAW global\n"
   "memory and write to an HBOOK file.)";

int main( int argc, char* argv[] ) {

   //
   // Create a Qt application and let it remove the Qt-related entries from the
   // command line arguments:
   //
   QApplication app( argc, argv );
   app.setFont( gui::DefaultFont() );

   //
   // Read the rest of the command line arguments:
   //
   TCLAP::CmdLine cmd( description, ' ', CDA_VERSION_STRING );
   TCLAP::ValueArg< int > verbosity( "v", "verbosity",
                                     "Level of output verbosity", false, 3,
                                     "code" );
   cmd.add( verbosity );
   TCLAP::ValueArg< std::string >
      config( "c", "config", "Name of an XML config file, or "
              "address of a config server", false, "", "filename/address");
   cmd.add( config );
   cmd.parse( argc, argv );

   //
   // Create a splash screen:
   //
   gui::SplashScreen splash( QPixmap( ":/img/splash.png" ),
                             Qt::WindowStaysOnTopHint |
                             Qt::X11BypassWindowManagerHint );
   splash.show();

   // Set the message server to a fixed address:
   msg::Sender::addAddress( Address( Const::MSG_SERVER_ADDRESS ) );

   // Logger object used to print messages directly from this function:
   msg::Logger logger( "cda-camac-daq" );

   //
   // Load all the available translations:
   //
   i18n::Loader trans_loader;
   if( ! trans_loader.loadTranslations() ) {
      logger << msg::FATAL << qApp->translate( "cda-camac-daq",
                                               "Couldn't load the translations!" )
             << msg::endmsg;
      return 1;
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
             << qApp->translate( "cda-camac-daq",
                                 "Didn't recognise verbosity level setting\n"
                                 "Terminating..." )
             << msg::endmsg;
      return 1;
   }

   //
   // Create and show the main window of the application:
   //
   CamacDAQWindow window( config.getValue().c_str(),
                          itr->second );
   window.show();
   splash.showMessage( qApp->translate( "cda-camac-daq",
                                        "CAMAC DAQ Ready" ),
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white );

   // Run the main Qt event loop:
   return app.exec();
}
