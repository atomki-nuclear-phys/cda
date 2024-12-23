// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_IDEVICE_H
#define CDA_CORE_DEVICE_IDEVICE_H

// Local include(s):
#include "IConfig.h"

namespace dev {

/**
 *  @short Base interface for all devices handled by CDA
 *
 *         Each device has to provide some common functions.
 *         These can't be put into dev::IConfig, as that class is used
 *         as base class not only for the device classes.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class IDevice : public virtual IConfig {

public:
   /// The type of the device
   /**
    * When reading and writing the configuration of a whole crate of
    * devices, the user of such a class does have to know a unique
    * ID for the object in question. This should be the same as the
    * value returned by Factory::deviceName().
    */
   virtual const QString& deviceName() const = 0;

   /// Get a unique identifier for the device
   /**
    * For CAMAC devices this is the slot number that the device is plugged
    * into. For VME devices we use the base address of the device. For CAEN
    * digitisers it's the ID used by the CAEN digitizer library.
    *
    * @returns A unique identifier for the device
    */
   virtual unsigned int getID() const = 0;
   /// Set a unique identifier for the device
   /**
    * @see dev::IDevice::getID()
    * @param value A unique identifier for the device
    */
   virtual void setID(unsigned int value) = 0;

};  // class IDevice

}  // namespace dev

#endif  // CDA_CORE_DEVICE_IDEVICE_H
