// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "CernlibDisk.h"
#include "CernlibHist.h"
#include "Gui.h"
#include "Readout.h"
#include "RootDisk.h"
#include "T4300bFactory.h"

namespace t4300b {

const QString& T4300bFactory::shortName() const {

   static const QString name("T4300B");
   return name;
}

const QString& T4300bFactory::longName() const {

   static const QString name(tr("LeCroy 4300B FERA QDC"));
   return name;
}

void* T4300bFactory::createDevice(const std::type_info& ti) const {

   if (ti == typeid(dev::CamacGui)) {

      return static_cast<dev::CamacGui*>(new Gui());

   } else if (ti == typeid(dev::ICamacReadout)) {

      return static_cast<dev::ICamacReadout*>(new Readout());

   } else if (ti == typeid(dev::ICernlibHist)) {

      return static_cast<dev::ICernlibHist*>(new CernlibHist());

   } else if (ti == typeid(dev::ICernlibDisk)) {

      return static_cast<dev::ICernlibDisk*>(new CernlibDisk());

   } else if (ti == typeid(dev::IRootDisk)) {

      return static_cast<dev::IRootDisk*>(new RootDisk());
   }

   return 0;
}

}  // namespace t4300b

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(t4300b, t4300b::T4300bFactory)
#endif
