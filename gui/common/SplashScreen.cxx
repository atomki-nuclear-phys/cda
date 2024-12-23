
// Qt include(s):
#include <QTimer>

// CDA include(s):
#include "CDAVersion.h"

// Local include(s):
#include "SplashScreen.h"

namespace gui {

SplashScreen::SplashScreen(const QPixmap& pixmap, Qt::WindowFlags f,
                           int timeout)
    : QSplashScreen(pixmap, f) {

   // Display the application's version:
   showMessage(tr("Version: %1").arg(CDA_VERSION_STRING),
               Qt::AlignHCenter | Qt::AlignTop, Qt::white);

   // Call the hide function after the timeout has elapsed:
   QTimer::singleShot(timeout * 1000, this, SLOT(hideSplash()));
}

void SplashScreen::hideSplash() {

   // Hide the splash screen:
   finish(this);
   return;
}

}  // namespace gui
