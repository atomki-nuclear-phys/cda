// $Id$

// System include(s):
#include <iostream>

// Qt include(s):
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/i18n/Loader.h"
#   include "cdacore/msg/Sender.h"
#   include "cdacore/common/Address.h"
#   include "cdadaq/moni/Histogram.h"
#else
#   include "i18n/Loader.h"
#   include "msg/Sender.h"
#   include "common/Address.h"
#   include "moni/Histogram.h"
#endif // Q_OS_DARWIN

int main( int argc, char* argv[] ) {

   // Set the destination of the messages:
   msg::Sender::addAddress( Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::VERBOSE );

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
   moni::Histogram hist( "Test histogram", 50, -200.0, -100.0 );
   //   hist.setXAxisStyle( moni::Histogram::Linear );
   hist.setYAxisStyle( moni::Histogram::Logarithmic );
   hist.fill( -118.0, 12010.4 );
   hist.fill( -130.5, 5.3 );
   hist.show();

   //
   // Run the application:
   //
   return app.exec();
}
