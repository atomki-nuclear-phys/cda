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
      virtual ev::Fragment* readEvent() const;

   private:
      caen::Digitizer m_digitizer; ///< Connection to the hardware

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Readout

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_READOUT_H
