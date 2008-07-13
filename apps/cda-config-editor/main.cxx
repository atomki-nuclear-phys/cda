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

// Qt include(s):
#include <QtCore/QtPlugin>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>
#include <QtCore/QLibraryInfo>
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Sender.h"
#else
#   include "msg/Sender.h"
#endif

// Local include(s):
#include "ConfigEditorWindow.h"

// Load all the static plugins:
Q_IMPORT_PLUGIN( t2228a )

int main( int argc, char* argv[] ) {

   //
   // Set the destination of the messages:
   //
   msg::Sender::addAddress( msg::Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::INFO );

   //
   // Instantiate the back-bone of the Qt graphical application:
   //
   QApplication app( argc, argv );

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
