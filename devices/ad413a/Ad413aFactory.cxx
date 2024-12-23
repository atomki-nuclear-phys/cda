// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Ad413aFactory.h"
#include "CernlibDisk.h"
#include "CernlibHist.h"
#include "Gui.h"
#include "Readout.h"
#include "RootDisk.h"

namespace ad413a {

const QString& Ad413aFactory::shortName() const {

   static const QString name("AD413A");
   return name;
}

const QString& Ad413aFactory::longName() const {

   static const QString name(tr("ORTEC AD413A Quad 8k ADC"));
   return name;
}

void* Ad413aFactory::createDevice(const std::type_info& ti) const {

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

}  // namespace ad413a

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(ad413a, ad413a::Ad413aFactory)
#endif
