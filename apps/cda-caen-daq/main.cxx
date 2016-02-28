// $Id$
/**
 *   @file apps/cda-caen-daq/main.cxx
 *  @short Main file for the cda-caen-daq application
 *
 *         This file stores the code that runs the cda-caen-daq application.
 *         It is rather simple, it reads a few simple command line options,
 *         then sets up CaenDAQWindow to do the rest of the job.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */

// Qt include(s):
#include <QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdacore/msg/Sender.h"
#   include "cdacore/tclap/CmdLine.h"
#   include "cdacore/i18n/Loader.h"
#   include "cdagui/common/DefaultFont.h"
#   include "cdagui/common/SplashScreen.h"
#else
#   include "msg/Logger.h"
#   include "msg/Sender.h"
#   include "tclap/CmdLine.h"
#   include "i18n/Loader.h"
#   include "common/DefaultFont.h"
#   include "common/SplashScreen.h"
#endif

// Local include(s):
#include "CaenDAQWindow.h"
#include "Constants.h"

// Description for the executable:
static const char* description =
   "Main DAQ program for running CDA on a single machine that does"
   "all the work in a CAEN data taking session.";

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
   TCLAP::CmdLine cmd( description );
   TCLAP::ValueArg< int > verbosity( "v", "verbosity",
                                     "Level of output verbosity", false, 3,
                                     "code" );
   cmd.add( verbosity );
   TCLAP::ValueArg< std::string >
      config( "c", "config", "Name of an XML config file, or "
              "address of a config server", false, "", "filename/address");
   cmd.add( config );
   cmd.parse( argc, argv );

   // Set the message server to a fixed address:
   msg::Sender::addAddress( Address( Const::MSG_SERVER_ADDRESS ) );

   // Logger object used to print messages directly from this function:
   msg::Logger logger( "cda-caen-daq" );

   //
   // Create a splash screen:
   //
   gui::SplashScreen splash( QPixmap( ":/img/splash.png" ),
                             Qt::WindowStaysOnTopHint |
                             Qt::X11BypassWindowManagerHint );
   splash.show();

   //
   // Load all the available translations:
   //
   i18n::Loader trans_loader;
   if( ! trans_loader.loadTranslations() ) {
      logger << msg::FATAL << qApp->translate( "cda-caen-daq",
                                               "Couldn't load the translations!" )
             << msg::endmsg;
      return 1;
   }

   //
   // Translate the verbosity option:
   //
   msg::Level lvl = msg::INFO;
   switch( verbosity.getValue() ) {

   case 1:
      lvl = msg::VERBOSE;
      break;
   case 2:
      lvl = msg::DEBUG;
      break;
   case 3:
      lvl = msg::INFO;
      break;
   case 4:
      lvl = msg::WARNING;
      break;
   case 5:
      lvl = msg::ERROR;
      break;
   case 6:
      lvl = msg::FATAL;
      break;
   case 7:
      lvl = msg::ALWAYS;
      break;
   default:
      logger << msg::FATAL
             << qApp->translate( "cda-caen-daq",
                                 "Didn't recognise verbosity level setting\n"
                                 "Terminating..." )
             << msg::endmsg;
      return 1;
   }
   msg::Sender::instance()->setMinLevel( lvl );

   //
   // Create and show the main window of the application:
   //
   CaenDAQWindow window( config.getValue().c_str(), lvl );
   window.show();
   splash.showMessage( qApp->translate( "cda-caen-daq",
                                        "CAEN DAQ Ready" ),
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white );

   // Run the main Qt event loop:
   return app.exec();
}
