// $Id$
/**
 *   @file apps/cda-stat-server/main.cxx
 *  @short Main file for the Statistics Server application
 *
 *         This file stores the code that runs the cda-stat-server application.
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

// Local include(s):
#include "StatServerWindow.h"

int main( int argc, char* argv[] ) {

   //
   // Start a graphical Qt application:
   //
   QApplication app( argc, argv );

   //
   // Create and show the statistics server window:
   //
   StatServerWindow window;
   window.show();

   return app.exec();

}
