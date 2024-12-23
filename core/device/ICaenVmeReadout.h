// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_ICAENVMEREADOUT_H
#define CDA_CORE_DEVICE_ICAENVMEREADOUT_H

// System include(s):
#include <memory>

// Local include(s):
#include "../common/StatusCode.h"
#include "../event/Fragment.h"
#include "IDevice.h"

// Forward declaration(s):
namespace caen {
class VmeBus;
}

namespace dev {

/**
 *  @short Interface for reading out data from a CAEN VME device
 *
 *         This class describes the interface that the application
 *         reading out the information from the CAEN VME device can
 *         use to access the hardware.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class ICaenVmeReadout : virtual public IDevice {

public:
   /// Function initializing the device
   /**
    * This function should be called before starting the data
    * acquisition, to configure the device for the data
    * acquisition.
    *
    * Notice that CAEN VME devices are supposed to have their private
    * instances of caen::VmeDevice for communicating with their
    * hardware. But there's a single instance of caen::VmeBus that is needed
    * by caen::VmeDevice to be initialised...
    *
    * @param bus The caen::VmeBus instance to use in the data taking
    * @returns StatusCode with the outcome of the operation
    */
   virtual StatusCode initialize(const caen::VmeBus& bus) = 0;

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
    * device. Once it is called, cda-caen-vme-reader will go to looking for
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

   /// Get the number of events processed by the device so far
   /**
    * All CAEN VME devices collect events using their own gates. These should
    * be syncronised in the hardware, but it's not at all impossible for an
    * event to go missing in one or the other device.
    *
    * With this function the devices can be asked how many events they've
    * seen so far. So that as soon as the devices go out of sync, the data
    * acquisition could be reset, starting fresh.
    *
    * @returns The number of events processed by the device so far
    */
   virtual size_t eventsProcessed() const = 0;

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

};  // class ICaenVmeReadout

}  // namespace dev

#endif  // CDA_CORE_DEVICE_ICAENVMEREADOUT_H
