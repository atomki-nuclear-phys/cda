
// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "Hist.h"

namespace t2228a {

   Factory::Factory() {

   }

   QString Factory::deviceName() const {

      return "T2228A";

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

      return 0;

   }

} // namespace t2228a

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( t2228a, t2228a::Factory )
