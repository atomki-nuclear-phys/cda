// Dear emacs, this is -*- c++ -*-
#ifndef CDA_APPS_COMMON_READCONFIG_H
#define CDA_APPS_COMMON_READCONFIG_H

// Project include(s).
#include "common/StatusCode.h"
#include "device/IConfig.h"

// Qt include(s):
#include <QString>

namespace apps
{
   //
   // Make sure that the following Qt classes are available in the
   // @c apps namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE(QString);

   /// Helper function for configuring a device from an XML file
   ///
   /// @param source The name of an XML file, or the address of a configuration
   ///               server
   /// @param device The device/crate object to configure
   /// @return The usual StatusCode values
   ///
   StatusCode readConfig(const QString &source, dev::IConfig &device);

} // namespace apps

#endif // CDA_APPS_COMMON_READCONFIG_H
