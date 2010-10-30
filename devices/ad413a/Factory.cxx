// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "Hist.h"
#include "Disk.h"

namespace ad413a {

   Factory::Factory() {

   }

   QString Factory::shortName() const {

      return "AD413A";

   }

   QString Factory::longName() const {

      return "ORTEC AD413A Quad 8k ADC";

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

} // namespace ad413a

// Declare this plugin to Qt:
Q_EXPORT_PLUGIN2( ad413a, ad413a::Factory )
