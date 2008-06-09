
// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Factory.h"
#include "Gui.h"
#include "Readout.h"
#include "Hist.h"
#include "Disk.h"

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

      return new Readout();

   }

   dev::Hist* Factory::createHist() const {

      return new Hist();

   }

   dev::Disk* Factory::createDisk() const {

      return new Disk();

   }

} // namespace camac

Q_EXPORT_PLUGIN2( example, camac::Factory )
