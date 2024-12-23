// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_V812_V812FACTORY_H
#define CDA_DEVICES_V812_V812FACTORY_H

// Qt include(s):
#include <QtCore/QObject>

// CDA include(s):
#include "device/Factory.h"

/**
 *  @short Namespace for the V812 CFD device
 *
 *         This namespace holds all the classes that handle the a V812
 *         CAEN VME Constant Fraction Discriminator.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
namespace v812 {

//
// Make sure that the following Qt classes are available in the
// namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QObject);
using QT_PREPEND_NAMESPACE(QString);

/**
 *  @short Factory creating all the V812 objects
 *
 *         This factory is used when loading the V812 plugin to access
 *         all the classes provided by the plugin.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class V812Factory : public QObject, virtual public dev::Factory {

   Q_OBJECT
   Q_INTERFACES(dev::Factory)
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
   Q_PLUGIN_METADATA(IID "hu.atomki.CDA.dev.v812/1.0")
#endif

public:
   /// Short name of the device implemented in the plug-in
   virtual const QString& shortName() const;
   /// Long name of the device implemented in the plug-in
   virtual const QString& longName() const;

protected:
   /// Universal function for creating a device object of this type
   virtual void* createDevice(const std::type_info& ti) const;

};  // class V812Factory

}  // namespace v812

#endif  // CDA_DEVICES_V812_V812FACTORY_H
