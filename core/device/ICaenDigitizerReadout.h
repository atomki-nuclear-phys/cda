// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_ICAENDIGITIZERREADOUT_H
#define CDA_CORE_DEVICE_ICAENDIGITIZERREADOUT_H

// System include(s):
#include <memory>

// Local include(s):
#include "../common/StatusCode.h"
#include "../event/Fragment.h"
#include "IDevice.h"

namespace dev {

/**
 *  @short Interface for reading out data from a CAEN digitizer
 *
 *         This class describes the interface that the application
 *         reading out the information from the CAEN digitizer can
 *         use to access the hardware.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class ICaenDigitizerReadout : virtual public IDevice {

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
    * @returns StatusCode with the outcome of the operation
    */
   virtual StatusCode initialize() = 0;

   /// Function "finalizing" the device
   /**
    * At the end of data taking the devices need to clean up after
    * themseves because of the way the CAEN Digitizer library works.
    * They should do that in this function.
    *
    * @returns StatusCode with the outcome of the operation
    */
   virtual StatusCode finalize() = 0;

   /// Function enabling data acquisition in the device
   /**
    * This is the function that should finally enable the readout of the
    * device. Once it is called, cda-caen-readout will go to looking for
    * collected events right away.
    *
    * @returns StatusCode with the outcome of the operation
    */
   virtual StatusCode start() = 0;

   /// Function disabling data acquisition in the device
   /**
    * Not sure how useful this function is going to be, as data acquisition
    * usually only needs to stop at the end of data taking, where the quick
    * stop is not that important. So it could be enough if finalize() would
    * take care of also stopping the acquisition.
    *
    * But still, for the moment let's keep it in the interface...
    *
    * @returns StatusCode with the outcome of the operation
    */
   virtual StatusCode stop() = 0;

   /// Function reading a single event from the device
   /**
    * This function is used to read out a single event from the
    * device.
    *
    * If there is no event available from the device, the function
    * should stall until an event becomes available. (Either by
    * waiting for an interrupt, or polling for new data.)
    *
    * Note that the caller of the function is responsible for
    * eventually deleting the received object.
    *
    * @returns The event fragment coming from this device
    */
   virtual std::unique_ptr<ev::Fragment> readEvent() = 0;

};  // class ICaenDigitizerReadout

}  // namespace dev

#endif  // CDA_CORE_DEVICE_ICAENDIGITIZERREADOUT_H
