// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_IVMEREADOUT_H
#define CDA_CORE_DEVICE_IVMEREADOUT_H

// Local include(s):
#include "IDevice.h"
#include "../event/Fragment.h"
#include "../common/UniquePtr.h"

// Forward declaration(s):
namespace vme {
   class VmeBus;
}

namespace dev {

   /**
    *  @short Interface for reading out the data from a VME device
    *
    *         This class describes the interface through which the code
    *         interacts with VME readout devices.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class IVmeReadout : virtual public IDevice {

   public:
      /// Function starting the readout of the device
      virtual bool start( vme::VmeBus& bus ) const = 0;
      /// Function stopping the readout of the device
      virtual bool stop( vme::VmeBus& bus ) const = 0;

      /// Function initializing the device
      /**
       * This function should be called before starting the data readout
       * from the device. It should reset the device and prepare it for
       * the data acquisition.
       *
       * @param bus The object to access the VME bus with
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( vme::VmeBus& bus ) const = 0;

      /// Function reading the current data from the device
      /**
       * The device is supposed to present its latest data when calling
       * this function.
       *
       * Note that the caller of the function is responsible for
       * eventually deleting the received object.
       *
       * @param bus The object to access the VME bus with
       * @returns The event fragment coming from this device
       */
      virtual UniquePtr< ev::Fragment >::Type
      readEvent( vme::VmeBus& bus ) const = 0;

   }; // class IVmeReadout

} // namespace dev

#endif // CDA_CORE_DEVICE_IVMEREADOUT_H
