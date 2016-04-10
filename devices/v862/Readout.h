// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V862_READOUT_H
#define CDA_DEVICES_V862_READOUT_H

// System include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICaenVmeReadout.h"
#include "caen/VmeDevice.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace v862 {

   /// Class responsible for communicating with the V862 hardware module
   ///
   /// This class is used by the readout application to configure the V862
   /// device for data acquisition, and then read events from it.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date
   ///
   class Readout : public Device,
         public virtual dev::ICaenVmeReadout {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v862::Readout )

   public:
      /// Default constructor
      Readout();

      /// @name Functions inherited from the dev::ICaenVmeReadout interface
      /// @{

      /// Function initialising the device for data acquisition
      virtual bool initialize( const caen::VmeBus& bus );
      /// Function finalising the device's data acquisition session
      virtual bool finalize();

      /// Function starting data acquisition on the device
      virtual bool start();
      /// Function stopping the device's data acquisition
      virtual bool stop();

      /// Function reading a single event from the device
      virtual std::unique_ptr< ev::Fragment > readEvent();

      /// @}

   private:
      /// The object used in communicating with the hardware
      caen::VmeDevice m_vmeDevice;

      /// Current list of events already read from the device
      std::vector< caen::VmeDevice::DataEvent > m_events;

      /// A message logging object
      mutable msg::Logger m_logger;

   }; // class Readout

} // namespace v862

#endif // CDA_DEVICES_V862_READOUT_H