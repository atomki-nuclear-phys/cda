// $Id$
/**
 *   @file apps/cda-qt-monitoring/main.cxx
 *  @short Main file for the Qt DAQ Monitoring application
 *
 *         This file stores the code that runs the cda-qt-monitoring application.
 *         The file is very simple, it just creates one window and lets it do
 *         everything else...
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#   include "cdacore/i18n/Loader.h"
#   include "cdagui/common/DefaultFont.h"
#   include "cdagui/common/SplashScreen.h"
#else
#   include "msg/Sender.h"
#   include "i18n/Loader.h"
#   include "common/DefaultFont.h"
#   include "common/SplashScreen.h"
#endif

// Local include(s):
#include "QtMonitoringWindow.h"

int main( int argc, char* argv[] ) {

   //
   // Only display WARNING or more serious messages by this application:
   //
   msg::Sender::instance()->setMinLevel( msg::WARNING );

   //
   // Start a graphical Qt application:
   //
   QApplication app( argc, argv );
   app.setFont( gui::DefaultFont() );

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
      std::cerr << "Couldn't load the translations!" << std::endl;
      return 1;
   }

   //
   // Create and show the statistics server window:
   //
   QtMonitoringWindow window;
   window.show();
   splash.showMessage( "Qt DAQ Monitoring Ready",
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white );

   return app.exec();
}
