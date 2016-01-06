// $Id$

// Local include(s):
#include "VmeDevice.h"
#include "VmeBus.h"

namespace caen {

   VmeDevice::VmeDevice() {

   }

   bool VmeDevice::open( uint16_t address, DeviceType type,
                         const VmeBus& bus ) {

      // Return gracefully:
      return true;
   }

   bool VmeDevice::close() {

      // Return gracefully:
      return true;
   }

} // namespace caen
