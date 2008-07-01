// $Id$

// STL include(s):
#include <iostream>

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
