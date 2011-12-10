// $Id$

// Qt include(s):
#include <QtCore/QTimer>

// Local include(s):
#include "SplashScreen.h"

namespace gui {

   SplashScreen::SplashScreen( const QPixmap& pixmap, Qt::WindowFlags f,
                               int timeout )
      : QSplashScreen( pixmap, f ) {

      // Call the hide function after the timeout has elapsed:
      QTimer::singleShot( timeout * 1000, this,
                          SLOT( hideSplash() ) );
   }

   void SplashScreen::hideSplash() {

      // Hide the splash screen:
      finish( this );
      return;
   }

} // namespace gui
