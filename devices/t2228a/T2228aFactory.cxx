// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "CernlibDisk.h"
#include "CernlibHist.h"
#include "Gui.h"
#include "Readout.h"
#include "RootDisk.h"
#include "T2228aFactory.h"

namespace t2228a {

const QString& T2228aFactory::shortName() const {

   static const QString name("T2228A");
   return name;
}

const QString& T2228aFactory::longName() const {

   static const QString name(tr("LeCroy 2228A TDC"));
   return name;
}

void* T2228aFactory::createDevice(const std::type_info& ti) const {

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

}  // namespace t2228a

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(t2228a, t2228a::T2228aFactory)
#endif
