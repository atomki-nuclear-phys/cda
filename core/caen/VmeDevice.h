// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CAEN_VMEDEVICE_H
#define CDA_CORE_CAEN_VMEDEVICE_H

// System include(s):
#include <cinttypes>

// Local include(s):
#include "../common/Export.h"

namespace caen {

   // Forward declaration(s):
   class VmeBus;

   /**
    * @brief Interface to the CAENqtpSDK library
    *
    * This class is used to insulate the device code from the CAENqtpSDK
    * library, which is used to communicate with all the CAEN VME devices used
    * by the project. It pretty much just provides a C++ interface to the
    * library, providing some simulated data for testing purposes when the code
    * is built without that library.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT VmeDevice {

   public:
      /// Default constructor
      VmeDevice();

      /// Supported device types
      enum DeviceType {
         DEV_V775A = 0, ///< The board is a V775A
         DEV_V785A = 1, ///< The board is a V785A
         DEV_V792A = 2  ///< The board is a V792A
      };

      /// Open connection to a CAEN VME device
      bool open( uint16_t address, DeviceType type, const VmeBus& bus );
      /// Close the connection to the device
      bool close();

   }; // class VmeDevice

} // namespace caen

#endif // CDA_CORE_CAEN_VMEDEVICE_H
