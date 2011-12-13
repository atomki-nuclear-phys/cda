// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CAENREADOUT_H
#define CDA_CORE_DEVICE_CAENREADOUT_H

// CDA include(s):
#include "../event/Fragment.h"

// Local include(s):
#include "Device.h"

namespace dev {

   /**
    *  @short Interface for reading out data from a CAEN digitizer
    *
    *         This class describes the interface that the application
    *         reading out the information from the CAEN digitizer can
    *         use to access the hardware.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CaenReadout : virtual public Device {

   public:
      /// Function initializing the device
      /**
       * This function should be called before starting the data
       * acquisition, to configure the device for the data
       * acquisition.
       *
       * Notice that CAEN devices are supposed to have their private
       * instances of caen::Digitizer for communicating with their
       * hardware. So the function doesn't need to receive anything.
       *
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize() = 0;

      /// Function "finalizing" the device
      /**
       * At the end of data taking the devices need to clean up after
       * themseves because of the way the CAEN Digitizer library works.
       * They should do that in this function.
       *
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool finalize() = 0;

      /// Function reading a single event from the device
      /**
       * This function is used to read out a single event from the
       * device.
       *
       * If there is no event available from the device, the function
       * should stall until an event becomes available. (Either by
       * waiting for an interrupt, or polling for new data.)
       *
       * @returns The event fragment coming from this device
       */
      virtual ev::Fragment readEvent() const = 0;

   }; // class CaenReadout

} // namespace dev

#endif // CDA_CORE_DEVICE_CAENREADOUT_H
