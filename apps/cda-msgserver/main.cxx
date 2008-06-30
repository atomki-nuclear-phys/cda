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

   QFont font = app.font();
   font.setPointSize( 11 );
   app.setFont( font );

   //
   // Create and show the message server window:
   //
   MsgServerWindow window;
   window.show();

   return app.exec();

}
