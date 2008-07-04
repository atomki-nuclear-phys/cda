// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_DEVICE_H
#define CDA_CORE_DEVICE_DEVICE_H

// Local include(s):
#include "Config.h"

namespace dev {

   /**
    *  @short Base class for all CAMAC module handling classes
    *
    *         Each CAMAC module has to provide some common functions.
    *         These can't be put into dev::Config, as that class is used
    *         as base class not only for the CAMAC module classes.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Device : public virtual Config {

   public:
      /// Destructor
      virtual ~Device() {}

      /// The type of the device
      /**
       * When reading and writing the configuration of a whole crate of
       * devices, the user of such a class does have to know a unique
       * ID for the object in question. This should be the same as the
       * value returned by Factory::deviceName().
       */
      virtual QString type() const = 0;

      /// Get the slot of the device in the CAMAC crate
      /**
       * The slot numbering starts from 1, it should be the same at the
       * value that you see on the front of the CAMAC crate under the
       * module.
       *
       * @returns The slot of the CAMAC module
       */
      virtual int getSlot() const = 0;
      /// Set the slot of the device in the CAMAC crate
      /**
       * @see dev::Device::setSlot()
       * @param value The slot of the CAMAC module
       */
      virtual void setSlot( int value ) = 0;

   }; // class Device

} // namespace dev

#endif // CDA_CORE_DEVICE_DEVICE_H
