// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#else
#   include "device/Factory.h"
#endif

// Local include(s):
#include "Crate.h"

namespace conf {

   /**
    * The constructor has to create the base class in a fancy way. It
    * specifies that the base class should use dev::Readout kind of
    * devices, and that it should use the dev::Factory::createReadout
    * function to create these devices when reading a configuration.
    */
   Crate::Crate()
      : dev::Crate< dev::CernlibHist >() {

   }

} // namespace conf
