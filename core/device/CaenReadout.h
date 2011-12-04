// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CAENREADOUT_H
#define CDA_CORE_DEVICE_CAENREADOUT_H

// CDA include(s):
#include "../event/Fragment.h"

// Local include(s):
#include "Device.h"

// Forward declaration(s):
namespace caen {
   class Digitizer;
} // namespace caen

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
       * @param dgtz The object to access the CAEN digitizer with
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( caen::Digitizer& dgtz ) const = 0;

      /// Function reading a single event from the device
      /**
       * This function is used to read out a single event from the
       * device.
       *
       * @param dgtz The object to access the CAEN digitizer with
       * @returns The event fragment coming from this device
       */
      virtual ev::Fragment readEvent( caen::Digitizer& dgtz ) const = 0;

      /// Function returning the number of digitized events
      /**
       * The devices can have an internal buffer in which they store
       * digitized events. When an interrupt starts the event readout,
       * the readout should continue until there are no events left
       * in the device's buffer. This function can be used to ask the
       * device how many more events it has for readout.
       *
       * @param dgtz The object to access the CAEN digitizer with
       * @returns The number of events in the digitizer's memory
       */
      virtual int eventsAvailable( caen::Digitizer& dgtz ) const = 0;

   }; // class CaenReadout

} // namespace dev

#endif // CDA_CORE_DEVICE_CAENREADOUT_H
