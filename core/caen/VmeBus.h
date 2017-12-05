// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_CAEN_VMEBUS_H
#define CDA_CORE_CAEN_VMEBUS_H

// System include(s):
#include <cinttypes>
#include <csignal>

// Qt include(s):
#include <QCoreApplication>

// Local include(s):
#include "../msg/Logger.h"
#include "../common/Export.h"

namespace caen {

   /**
    * @brief Interface to the CAENVMELib library
    *
    * While the CAENDigitizer library makes use of CAENVMELib in the background,
    * opening a separate connection to each of the devices (as the one known
    * digitiser at the moment is connected through USB anyway), in order to
    * communicate with devices through a VX1718 bridge, all the devices must
    * use the same VME connection.
    *
    * This is what this class helps with. It wraps the functionality useful for
    * the CVT functions, from the CAENVMELib library.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDACORE_EXPORT VmeBus {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( caen::VmeBus )

   public:
      /// Default constructor
      VmeBus();
      /// Destructor
      ~VmeBus();

      /// CAEN VME board types
      enum BoardType {
         BOARD_V1718 = 0, ///< CAEN V1718 USB-VME bridge
         BOARD_V2718 = 1, ///< V2718 PCI-VME bridge with optical link
         BOARD_A2818 = 2, ///< PCI board with optical link
         BOARD_A2719 = 3  ///< Optical link piggy-back
      };

      /// Open the connection to the VME board
      bool open( BoardType type, short linkNumber, short boardNumber );
      /// Close the connection to the board
      bool close();

      /// Get the handle of the VME connection
      int32_t handle() const;

   private:
      int32_t m_handle; ///< Opaque handle to the VME device
      mutable msg::Logger m_logger; ///< Message logger object

#ifndef Q_OS_WIN
      /// Signals to block during hardware communication
      sigset_t m_blockedSignals;
#endif // Q_OS_WIN

   }; // class VmeBus

} // namespace caen

#endif // CDA_CORE_CAEN_VMEBUS_H
