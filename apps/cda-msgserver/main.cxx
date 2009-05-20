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

// Qt include(s):
#include <QtGui/QApplication>
#include <QtGui/QFont>

// Local include(s):
#include "MsgServerWindow.h"

int main( int argc, char* argv[] ) {

   //
   // Start a graphical Qt application:
   //
   QApplication app( argc, argv );

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
