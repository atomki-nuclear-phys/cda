// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "CernlibDisk.h"
#include "CernlibHist.h"
#include "Dt5740Factory.h"
#include "Gui.h"
#include "QtHist.h"
#include "Readout.h"
#include "RootDisk.h"

namespace dt5740 {

const QString& Dt5740Factory::shortName() const {

   static const QString name("DT5740");
   return name;
}

const QString& Dt5740Factory::longName() const {

   static const QString name(tr("CAEN DT5740 Desktop Digitizer"));
   return name;
}

void* Dt5740Factory::createDevice(const std::type_info& ti) const {

   if (ti == typeid(dev::CaenDigitizerGui)) {

      return static_cast<dev::CaenDigitizerGui*>(new Gui());

   } else if (ti == typeid(dev::ICernlibHist)) {

      return static_cast<dev::ICernlibHist*>(new CernlibHist());

   } else if (ti == typeid(dev::ICernlibDisk)) {

      return static_cast<dev::ICernlibDisk*>(new CernlibDisk());

   } else if (ti == typeid(dev::ICaenDigitizerReadout)) {

      return static_cast<dev::ICaenDigitizerReadout*>(new Readout());

   } else if (ti == typeid(dev::IRootDisk)) {

      return static_cast<dev::IRootDisk*>(new RootDisk());

   } else if (ti == typeid(dev::QtHist)) {

      return static_cast<dev::QtHist*>(new QtHist());
   }

   return 0;
}

}  // namespace dt5740

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(dt5740, dt5740::Dt5740Factory)
#endif
