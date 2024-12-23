// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_ICERNLIBDISK_H
#define CDA_CORE_DEVICE_ICERNLIBDISK_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QString>

// Local include(s):
#include "IDevice.h"

// Forward include(s):
namespace cernlib {
class NTupleMgr;
}  // namespace cernlib
namespace ev {
class Fragment;
}  // namespace ev

namespace dev {

/**
 *  @short Interface for writing an output file from the readout data
 *
 *         This interface needs to be implemented by the devices to be
 *         able to write their collected data into HBOOK ntuples.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class ICernlibDisk : virtual public IDevice {

public:
   /// Function initializing the device
   /**
    * Prepare the writing of the data read from this device. When
    * creating a row-wise ntuple using CERNLIB, one has to pass an
    * array of variable names to the function creating the ntuple.
    * Plus, the device object has to remember in which array element
    * to put which subaddress data.
    *
    * @param nmgr NTupleMgr object that can be used at the initialisation
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   virtual bool initialize(cernlib::NTupleMgr& nmgr) = 0;

   /// Function filling the ntuple
   /**
    * Write an event record to the output file. It should receive an
    * event fragment that was read out by the dev::Readout object.
    * This function doesn't write the data to the file itself, it just
    * sets the appropriate members of the array given to the function.
    *
    * @param fragment Event fragment that should be processed
    * @param nmgr NTupleMgr that should be filled with the data
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   virtual bool writeEvent(const ev::Fragment& fragment,
                           cernlib::NTupleMgr& nmgr) const = 0;

};  // class ICernlibDisk

}  // namespace dev

#endif  // CDA_CORE_DEVICE_ICERNLIBDISK_H
