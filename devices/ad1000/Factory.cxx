// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "Hist.h"
#include "Disk.h"

namespace ad1000 {

   QString Factory::shortName() const {

      return "AD1000";
   }

   QString Factory::longName() const {

      return "Atomki AD1000 ADC";
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

} // namespace ad1000

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( ad1000, ad1000::Factory )
