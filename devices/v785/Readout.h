// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V785_READOUT_H
#define CDA_DEVICES_V785_READOUT_H

// System include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "caen/VmeDevice.h"
#include "device/ICaenVmeReadout.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace v785 {

/// Class responsible for communicating with the V785 hardware module
///
/// This class is used by the readout application to configure the V785
/// device for data acquisition, and then read events from it.
///
/// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
///
/// $Revision$
/// $Date
///
class Readout : public Device, public virtual dev::ICaenVmeReadout {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(v785::Readout)

public:
   /// Default constructor
   Readout();

   /// @name Functions inherited from the dev::ICaenVmeReadout interface
   /// @{

   /// Function initialising the device for data acquisition
   virtual StatusCode initialize(const caen::VmeBus& bus);
   /// Function finalising the device's data acquisition session
   virtual StatusCode finalize();

   /// Function starting data acquisition on the device
   virtual StatusCode start();
   /// Function stopping the device's data acquisition
   virtual StatusCode stop();

   /// Get the number of events processed by the device so far
   virtual size_t eventsProcessed() const;

   /// Function reading a single event from the device
   virtual std::unique_ptr<ev::Fragment> readEvent();

   /// @}

private:
   /// The object used in communicating with the hardware
   caen::VmeDevice m_vmeDevice;

   /// Current list of events already read from the device
   std::vector<caen::VmeDevice::DataEvent> m_events;

   /// Number of events processed by the device so far
   size_t m_eventsProcessed;

   /// A message logging object
   mutable msg::Logger m_logger;

};  // class Readout

}  // namespace v785

#endif  // CDA_DEVICES_V785_READOUT_H
