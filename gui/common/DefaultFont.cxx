// $Id$

// Local include(s):
#include "DefaultFont.h"

namespace gui {

   /**
    * I prefer sans serif fonts by default, so Qt is supposed to look for
    * Helvetica if it's available on the system.
    */
   DefaultFont::DefaultFont()
      : QFont( "Helvetica [Cronyx]", 10 ) {

   }

} // namespace gui
