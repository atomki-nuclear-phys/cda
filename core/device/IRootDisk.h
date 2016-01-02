// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_IROOTDISK_H
#define CDA_CORE_DEVICE_IROOTDISK_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QString>

// Local include(s):
#include "IDevice.h"

// Forward include(s):
namespace root {
   class NTupleMgr;
} // namespace cernlib
namespace ev {
   class Fragment;
} // namespace ev

namespace dev {

   /**
    *  @short Interface for writing an output file from the readout data
    *
    *         This interface is used to write a ROOT file from the data
    *         collected by the CDA devices.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class IRootDisk : virtual public IDevice {

   public:
      /// Function initializing the device
      /**
       * The device has to declare its member variables that it wants to write
       * out during the event processing.
       *
       * @param nmgr NTupleMgr object that can be used at the initialisation
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( root::NTupleMgr& nmgr ) = 0;

      /// Function filling the ntuple
      /**
       * Write an event record to the output file. It should receive an
       * event fragment that was read out by the dev::Readout object.
       * This function doesn't write the data to the file itself, it just
       * sets its members to the value that it wants to write to the file.
       *
       * @param fragment Event fragment that should be processed
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool writeEvent( const ev::Fragment& fragment ) const = 0;

   }; // class IRootDisk

} // namespace dev

#endif // CDA_CORE_DEVICE_IROOTDISK_H
