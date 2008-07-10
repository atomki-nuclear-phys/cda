// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_DISK_H
#define CDA_CORE_DEVICE_DISK_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QString>

// Local include(s):
#include "Device.h"

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
   class Disk : virtual public Device {

   public:
      /// Function initializing the device
      /**
       * Prepare the writing of the data read from this device. When
       * creating a row-wise ntuple using CERNLIB, one has to pass an
       * array of variable names to the function creating the ntuple.
       * Plus, the device object has to remember in which array element
       * to put which subaddress data.
       *
       * @param counter Variable giving the array index of the next free
       *                array block
       * @param names Variable that should be filled with the names of the
       *              subaddresses
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( unsigned int& counter,
                               std::vector< QString >& names ) = 0;
      /// Function filling the histograms
      /**
       * Write an event record to the output file. It should receive an
       * event fragment that was read out by the dev::Readout object.
       * This function doesn't write the data to the file itself, it just
       * sets the appropriate members of the array given to the function.
       *
       * @param fragment Event fragment that should be processed
       * @param data Array that should be filled with the event data
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool writeEvent( const ev::Fragment& fragment,
                               float*& data ) const = 0;

   }; // class Disk

} // namespace dev

#endif // CDA_CORE_DEVICE_DISK_H
