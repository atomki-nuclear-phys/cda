
// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "Hist.h"
#include "Disk.h"

namespace ad2249a {

   Factory::Factory() {

   }

   QString Factory::shortName() const {

      return "AD2249A";

   }

   QString Factory::longName() const {

      return "LeCroy 2249A ADC";

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

} // namespace ad2249a

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( ad2249a, ad2249a::Factory )
