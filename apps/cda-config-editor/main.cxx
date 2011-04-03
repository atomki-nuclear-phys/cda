// $Id$
/**
 *   @file apps/cda-config-editor/main.cxx
 *  @short Main file for the configuration editor application
 *
 *         This file stores the small amount of code that runs the
 *         cda-config-editor application. It basically just creates a
 *         ConfigEditorWindow window and lets that take charge of things.
 *
 * @author Attila Krasznahorkay Jr.
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
#else
#   include "msg/Sender.h"
#   include "i18n/Loader.h"
#endif

// Local include(s):
#include "ConfigEditorWindow.h"

int main( int argc, char* argv[] ) {

   //
   // Only display WARNING or more serious messages by this application:
   //
   msg::Sender::instance()->setMinLevel( msg::WARNING );

   //
   // Instantiate the back-bone of the Qt graphical application:
   //
   QApplication app( argc, argv );

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

   //
   // Run the application:
   //
   return app.exec();
}
