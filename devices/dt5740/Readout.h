// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_READOUT_H
#define CDA_DEVICES_DT5740_READOUT_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CaenReadout.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CaenReadout.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace dt5740 {

   /**
    *  @short Class responsible for reading data from a single DT5740
    *
    *         This class is used by cda-caen-readout to read events from
    *         a single DT5740, and forward all the data to the other applications
    *         of CDA.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Readout : public virtual dev::CaenReadout,
                   public virtual Device {

   public:
      /// Constructor
      Readout();
      /// Destructor
      ~Readout();

      /// Function initializing the device
      virtual bool initialize();
      /// Function "finalizing" the device
      virtual bool finalize();

      /// Function enabling data acquisition in the device
      virtual bool start();
      /// Function disabling data acquisition in the device
      virtual bool stop();

      /// Function reading a single event from the device
      virtual ev::Fragment* readEvent();

   private:
      /// Get the external trigger mode
      caen::Digitizer::TriggerMode extTrigMode() const;
      /// Get the trigger mode of a channel group
      static caen::Digitizer::TriggerMode trigMode( const GroupConfig& group );
      /// Get the bit-pattern for the group configuration register
      uint32_t groupConfReg() const;
      /// Get the bit-pattern for the acquisition control register
      uint32_t acqControlReg() const;
      /// Clean up the allocated memory
      bool cleanup();

      /// Address of a test register on the device
      static const uint32_t REG_SCRATCH = 0xef20;
      /// Address of the group configuration register
      static const uint32_t REG_GROUP_CONFIG = 0x8000;
      /// Address of the event number register
      static const uint32_t REG_EVENT_STORED = 0x812c;
      /// Address of the acquisition control register
      static const uint32_t REG_ACQ_CONTROL = 0x8100;

      caen::Digitizer m_digitizer; ///< Connection to the hardware

      char* m_buffer; ///< Event buffer used during data readout
      uint32_t m_bufferSize; ///< Total size of the event buffer
      uint32_t m_eventSize; ///< Size of the current payload in the readout buffer
      uint32_t m_numEvents; ///< Number of events in the current readout buffer
      uint32_t m_currentEvent; ///< Current event to be read out from the buffer
      caen::Digitizer::EventInfo m_eventInfo; ///< Event information
      caen::Digitizer::EventData16Bit m_event; ///< Event data

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Readout

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_READOUT_H
