// $Id$

// System include(s):
#include <iostream>

// Qt include(s):
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/i18n/Loader.h"
#   include "cdagui/moni/Histogram.h"
#else
#   include "i18n/Loader.h"
#   include "moni/Histogram.h"
#endif // Q_OS_DARWIN

int main( int argc, char* argv[] ) {

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
   moni::Histogram hist( "Test histogram", 50, 0.0, 50.0 );
   hist.fill( 18.0, 12.4 );
   hist.fill( 30.5, 5.3 );
   hist.show();

   //
   // Run the application:
   //
   return app.exec();
}
