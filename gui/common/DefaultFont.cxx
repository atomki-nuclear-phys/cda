// $Id$

// Local include(s):
#include "DefaultFont.h"

namespace gui {

   /**
    * I prefer sans serif fonts by default, so Qt is supposed to look for
    * Helvetica if it's available on the system.
    */
   DefaultFont::DefaultFont()
#ifdef Q_OS_DARWIN
      : QFont( "Helvetica [Cronyx]", 12 ) {
#else
      : QFont( "Helvetica [Cronyx]", 10 ) {
#endif // Q_OS_DARWIN

   }

} // namespace gui
