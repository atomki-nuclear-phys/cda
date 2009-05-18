// $Id$

// Qt include(s):
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdagui/stat/RateWindow.h"
#else
#   include "stat/RateWindow.h"
#endif

int main( int argc, char* argv[] ) {

   QApplication app( argc, argv );

   stat::RateWindow widget;
   widget.show();

   /*
   widget.setNewRate( 10.5 );
   widget.setNewRate( 11.5 );
   widget.setNewRate( 12.5 );
   widget.setNewRate( 13.5 );
   widget.setNewRate( 14.5 );
   */

   return app.exec();

}
