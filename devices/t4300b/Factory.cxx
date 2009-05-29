// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "Hist.h"
#include "Disk.h"

namespace t4300b {

   Factory::Factory() {

   }

   QString Factory::shortName() const {

      return "T4300B";

   }

   QString Factory::longName() const {

      return "LeCroy 4300B FERA QDC";

   }

   dev::Gui* Factory::createGui() const {

      return new Gui();

   }

   dev::Readout* Factory::createReadout() const {

      return new Readout();

   }

   dev::Hist* Factory::createHist() const {

      return new Hist();

   }

   dev::Disk* Factory::createDisk() const {

      return new Disk();

   }

} // namespace t4300b

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( t4300b, t4300b::Factory )
