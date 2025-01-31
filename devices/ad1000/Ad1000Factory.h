// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_AD1000FACTORY_H
#define CDA_DEVICES_AD1000_AD1000FACTORY_H

// Qt include(s):
#include <QtCore/QObject>

// CDA include(s):
#include "device/Factory.h"

/**
 *  @short Namespace for the AD1000 ADC device
 *
 *         This namespace holds all the classes that handle the AD1000
 *         CAMAC ADC device.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace ad1000 {

//
// Make sure that the following Qt classes are available in the
// DEV namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QObject);
using QT_PREPEND_NAMESPACE(QString);

/**
 *  @short Factory creating all the AD1000 objects
 *
 *         This factory is used when loading the AD1000 plugin to access
 *         all the classes provided by the plugin.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class Ad1000Factory : public QObject, virtual public dev::Factory {

   Q_OBJECT
   Q_INTERFACES(dev::Factory)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
   Q_PLUGIN_METADATA(IID "hu.atomki.CDA.dev.ad1000/1.0")
#endif

public:
   /// Get the shortened name of the device
   virtual const QString& shortName() const;
   /// Get the long name of the device
   virtual const QString& longName() const;

protected:
   /// Universal function for creating a device object of this type
   virtual void* createDevice(const std::type_info& ti) const;

};  // class Ad1000Factory

}  // namespace ad1000

#endif  // CDA_DEVICES_AD1000_AD1000FACTORY_H
