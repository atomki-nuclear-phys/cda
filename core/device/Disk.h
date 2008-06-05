// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_DISK_H
#define CDA_CORE_DEVICE_DISK_H

// Local include(s):
#include "Config.h"

// Forward include(s):
namespace ev {
   class Fragment;
} // namespace ev

namespace dev {

   /**
    *  @short Interface for writing an output file from the readout data
    *
    *         This is a first guess at what the interface of the disk writing
    *         classes should look like. There should be a function that
    *         prepares the writing of the output file, and another one
    *         that can be called for each event to write an event record
    *         to the file.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Disk : virtual public Config {

   public:
      /// Function initializing the device
      /**
       * Prepare the writing of the data read from this device.
       *
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize() = 0;
      /// Function filling the histograms
      /**
       * Write an event record to the output file. It should receive an
       * event fragment that was read out by the dev::Readout object.
       * How this object receives this data is for the future to see...
       *
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool writeEvent( const ev::Fragment& fragment ) = 0;

   }; // class Disk

} // namespace dev

#endif // CDA_CORE_DEVICE_DISK_H
