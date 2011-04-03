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

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/i18n/Loader.h"
#else
#   include "i18n/Loader.h"
#endif

// Local include(s):
#include "StatServerWindow.h"

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
   // Create and show the statistics server window:
   //
   StatServerWindow window;
   window.show();

   return app.exec();
}
