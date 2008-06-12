// $Id$

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QtGui/QApplication>

// Local include(s):
#include "MsgServerWindow.h"

int main( int argc, char* argv[] ) {

   //
   // Start a graphical Qt application:
   //
   QApplication app( argc, argv );

   //
   // Create and show the message server window:
   //
   MsgServerWindow window;
   window.show();

   return app.exec();

}
