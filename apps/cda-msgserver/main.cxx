// $Id$
/**
 *   @file apps/cda-msgserver/main.cxx
 *  @short Main file for the Message Server application
 *
 *         This file stores the code that runs the cda-msgserver application.
 *         The file is very simple, it just creates one window and lets it do
 *         everything else...
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
#include <QtGui/QFont>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/i18n/Loader.h"
#else
#   include "i18n/Loader.h"
#endif

// Local include(s):
#include "MsgServerWindow.h"

int main( int argc, char* argv[] ) {

   //
   // Start a graphical Qt application:
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
   // Set the default application font size. Different font sizes look
   // good on SLC4 and Leopard...
   //
   QFont font = app.font();
#ifdef Q_OS_DARWIN
   //   font.setPointSize( 12 );
#else
   font.setPointSize( 10 );
#endif // Q_OS_DARWIN
   app.setFont( font );

   //
   // Create and show the message server window:
   //
   MsgServerWindow window;
   window.show();

   return app.exec();

}
