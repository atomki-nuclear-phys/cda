
// Qt include(s):
#include <QtCore/QtPlugin>

// CDA include(s):
#include "device/Readout.h"
#include "device/Hist.h"
#include "device/Disk.h"

// Local include(s):
#include "Factory.h"
#include "Gui.h"

namespace camac {

   Factory::Factory()
      : m_logger( "Example::Factory" ) {

   }

   QString Factory::deviceName() const {

      return "Example";

   }

   dev::Gui* Factory::createGui( QWidget* parent,
                                 Qt::WindowFlags flags ) const {

      return new Gui( parent, flags );

   }

   dev::Readout* Factory::createReadout() const {

      return 0;

   }

   dev::Hist* Factory::createHist() const {

      return 0;

   }

   dev::Disk* Factory::createDisk() const {

      return 0;

   }

} // namespace camac

Q_EXPORT_PLUGIN2( example, camac::Factory )
