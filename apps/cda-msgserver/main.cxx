// $Id$
/**
 *   @file apps/cda-msgserver/main.cxx
 *  @short Main file for the Message Server application
 *
 *         This file stores the code that runs the cda-msgserver application.
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
#include <QApplication>

// CDA include(s):
#include "common/DefaultFont.h"
#include "common/SplashScreen.h"
#include "i18n/Loader.h"

// Local include(s):
#include "MsgServerWindow.h"

int main(int argc, char* argv[]) {

   //
   // Start a graphical Qt application:
   //
   QApplication app(argc, argv);
   app.setFont(gui::DefaultFont());

   //
   // Create a splash screen:
   //
   gui::SplashScreen splash(
       QPixmap(":/img/splash.png"),
       Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
   splash.show();

   //
   // Load all the available translations:
   //
   i18n::Loader trans_loader;
   if (!trans_loader.loadTranslations()) {
      std::cerr << "Couldn't load the translations!" << std::endl;
      return 1;
   }

   //
   // Create and show the message server window:
   //
   MsgServerWindow window;
   window.show();
   splash.showMessage(qApp->translate("cda-msgserver", "Message Server Ready"),
                      Qt::AlignHCenter | Qt::AlignBottom, Qt::white);

   return app.exec();
}
