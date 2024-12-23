// $Id$

// Qt include(s):
#include <QtCore/QtPlugin>

// Local include(s):
#include "Ad2249aFactory.h"
#include "CernlibDisk.h"
#include "CernlibHist.h"
#include "Gui.h"
#include "QtHist.h"
#include "Readout.h"
#include "RootDisk.h"

namespace ad2249a {

const QString& Ad2249aFactory::shortName() const {

   static const QString name("AD2249A");
   return name;
}

const QString& Ad2249aFactory::longName() const {

   static const QString name(tr("LeCroy 2249A ADC"));
   return name;
}

void* Ad2249aFactory::createDevice(const std::type_info& ti) const {

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

   } else if (ti == typeid(dev::QtHist)) {

      return static_cast<dev::QtHist*>(new QtHist());
   }

   return 0;
}

}  // namespace ad2249a

// Declare this plugin to Qt:
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(ad2249a, ad2249a::Ad2249aFactory)
#endif
