// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_ICAMACREADOUT_H
#define CDA_CORE_DEVICE_ICAMACREADOUT_H

// Local include(s):
#include "IDevice.h"
#include "../common/UniquePtr.h"
#include "../event/Fragment.h"

// Forward declaration(s):
namespace camac {
   class Crate;
}

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
   class ICamacReadout : virtual public IDevice {

   public:
      /// Function initializing the device
      /**
       * This function should be called before starting the data readout
       * from the device. It should reset the device and prepare it for
       * the data acquisition.
       *
       * @param crate The object to access the CAMAC crate with
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( camac::Crate& crate ) const = 0;

      /// Function reading the current data from the device
      /**
       * The device creates an event fragment object in memory, and passes
       * that back to the caller. It's the caller's responsibility to delete
       * the object from the heap.
       *
       * @param crate The object to access the CAMAC crate with
       * @returns The event fragment coming from this device
       */
      virtual UniquePtr< ev::Fragment >::Type
      readEvent( camac::Crate& crate ) const = 0;

      /// Clear the module to receive a new event
      /**
       * This function is called after all the modules have been read out
       * to clear the data from the modules and make them ready to
       * receive a new event.
       *
       * @param crate The object to access the CAMAC crate with
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool clear( camac::Crate& crate ) const = 0;

   }; // class ICamacReadout

} // namespace dev

#endif // CDA_CORE_DEVICE_ICAMACREADOUT_H
