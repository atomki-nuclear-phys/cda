// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V792_V792FACTORY_H
#define CDA_DEVICES_V792_V792FACTORY_H

// Qt include(s):
#include <QtCore/QObject>

// CDA include(s):
#include "device/Factory.h"

/**
 *  @short Namespace for the V792 QDC device
 *
 *         This namespace holds all the classes that handle the a V792
 *         CAEN VME QDC device.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace v792 {

//
// Make sure that the following Qt classes are available in the
// DEV namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QObject);
using QT_PREPEND_NAMESPACE(QString);

/**
 *  @short Factory creating all the V792 objects
 *
 *         This factory is used when loading the V792 plugin to access
 *         all the classes provided by the plugin.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class V792Factory : public QObject, virtual public dev::Factory {

   Q_OBJECT
   Q_INTERFACES(dev::Factory)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
   Q_PLUGIN_METADATA(IID "hu.atomki.CDA.dev.v792/1.0")
#endif

public:
   /// Short name of the device implemented in the plug-in
   virtual const QString& shortName() const;
   /// Long name of the device implemented in the plug-in
   virtual const QString& longName() const;

protected:
   /// Universal function for creating a device object of this type
   virtual void* createDevice(const std::type_info& ti) const;

};  // class V792Factory

}  // namespace v792

#endif  // CDA_DEVICES_V792_V792FACTORY_H
