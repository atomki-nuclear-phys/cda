// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_READOUT_H
#define CDA_CORE_DEVICE_READOUT_H

// CDA include(s):
#include "../event/Fragment.h"

// Local include(s):
#include "Device.h"

// Forward declaration(s):
namespace camac {
   class Crate;
} // namespace camac

namespace dev {

   /**
    *  @short Interface for reading out data from a CAMAC device
    *
    *         This class describes the interface that the application
    *         reading out the physical CAMAC device can call on the device
    *         object. It's just a first guess of what the interface could
    *         be like. Will probably change drastically later on...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Readout : virtual public Device {

   public:
      /// Function initializing the device
      /**
       * This function should be called before starting the data readout
       * from the device. It should reset the device and prepare it for
       * the data acquisition. Probably should receive a pointer/reference
       * to some class with which it can access the CAMAC crate...
       *
       * @param crate The object to access the CAMAC crate with
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( camac::Crate& crate ) const = 0;
      /// Function reading the current data from the device
      /**
       * I'm just guessing here. It would probably make sense to define
       * an "event" structure. Then the whole event could be transmitted
       * between the processes. (Through a fifo or whatever...)
       *
       * @param crate The object to access the CAMAC crate with
       * @returns The event fragment coming from this device?
       */
      virtual ev::Fragment readEvent( camac::Crate& crate ) const = 0;

   }; // class Readout

} // namespace dev

#endif // CDA_CORE_DEVICE_READOUT_H
