// $Id$

// CDA include(s):
#include <QtGui/QApplication>

// Local include(s):
#include "DefaultFont.h"

namespace gui {

   /**
    * The application uses an ugly Times font on SLC5. This font makes
    * sure that the GUI applications look nice on Linux in general.
    */
   DefaultFont::DefaultFont()
#ifdef Q_OS_DARWIN
      // Don't modify the application default on Mac OS X:
      : QFont( QApplication::font() ) {
#else
      // On other platforms use a 10pt Helvetica font:
      : QFont( "Helvetica [Cronyx]", 10 ) {
#endif // Q_OS_DARWIN

   }

} // namespace gui
