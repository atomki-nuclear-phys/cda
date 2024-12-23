// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "CernlibDisk.h"
#include "CernlibHist.h"
#include "Gui.h"
#include "QtHist.h"
#include "Readout.h"
#include "RootDisk.h"
#include "V775Factory.h"

namespace v775 {

const QString& V775Factory::shortName() const {

   static const QString name("V775");
   return name;
}

const QString& V775Factory::longName() const {

   static const QString name(tr("CAEN V775 TDC"));
   return name;
}

void* V775Factory::createDevice(const std::type_info& ti) const {

   if (ti == typeid(dev::ICaenVmeReadout)) {

      return static_cast<dev::ICaenVmeReadout*>(new Readout());

   } else if (ti == typeid(dev::CaenVmeGui)) {

      return static_cast<dev::CaenVmeGui*>(new Gui());

   } else if (ti == typeid(dev::IRootDisk)) {

      return static_cast<dev::IRootDisk*>(new RootDisk());

   } else if (ti == typeid(dev::QtHist)) {

      return static_cast<dev::QtHist*>(new QtHist());

   } else if (ti == typeid(dev::ICernlibHist)) {

      return static_cast<dev::ICernlibHist*>(new CernlibHist());

   } else if (ti == typeid(dev::ICernlibDisk)) {

      return static_cast<dev::ICernlibDisk*>(new CernlibDisk());
   }

   return 0;
}

}  // namespace v775

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(v775, v775::V775Factory)
#endif
