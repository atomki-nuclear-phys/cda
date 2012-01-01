// $Id$
/**
 *   @file apps/cda-config-editor/main.cxx
 *  @short Main file for the configuration editor application
 *
 *         This file stores the small amount of code that runs the
 *         cda-config-editor application. It basically just creates a
 *         ConfigEditorWindow window and lets that take charge of things.
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
#include <QtGui/QPixmap>

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
#include "ConfigEditorWindow.h"
#include "../win32/plugins.h"

int main( int argc, char* argv[] ) {

   //
   // Only display WARNING or more serious messages by this application:
   //
   msg::Sender::instance()->setMinLevel( msg::WARNING );

   //
   // Instantiate the back-bone of the Qt graphical application:
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
   // Create and show the configuration editor window:
   //
   ConfigEditorWindow editor;
   editor.show();
   splash.showMessage( qApp->translate( "cda-config-editor",
                                        "Configuration Editor Ready" ),
                       Qt::AlignHCenter | Qt::AlignBottom,
                       Qt::white );

   //
   // Run the application:
   //
   return app.exec();
}
