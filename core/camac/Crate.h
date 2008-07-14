// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CAMAC_CRATE_H
#define CDA_CORE_CAMAC_CRATE_H

// System include(s):
#include <stdint.h>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CAMAC library include(s):
#ifndef TESTING
#   include "libcc32.h"
#endif

// CDA include(s):
#include "../msg/Logger.h"

/**
 *  @short Namespace for the CAMAC crate accessor class(es)
 *
 *         It seemed elegant to name the interface class accessing the CAMAC
 *         crate camac::Crate. Hence I created this namespace. Not sure if
 *         it will hold any other classes though...
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace camac {

   /**
    *  @short Class for accessing the CAMAC crate
    *
    *         This class is a wrapper around the pcicc32 driver. It is
    *         necessary to be able to develop the code on machines not
    *         connected to the CAMAC hardware. The class can be compiled
    *         in a "TESTING" mode in which it will simulate the behaviour
    *         of a CAMAC crate without actually using the pcicc32 driver.
    *
    *         This class is supposed to be fast. For this reason most of the
    *         log messages are only printed in TESTING mode. They could
    *         take up too much time to handle in real running even if they
    *         are not shown to the user.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Crate {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( camac::Crate )

   public:
      /// Constructor with device driver name
      Crate( const char* dev_path = "/dev/cc32_1" );
      /// Destructor
      ~Crate();

      /// Open the CAMAC crate
      bool open( const char* dev_path = 0 );
      /// Close the CAMAC crate
      bool close();

      /// Read 16-bits of information from the crate
      uint16_t readWord( unsigned int N, unsigned int A, unsigned int F );
      /// Read 24-bits of information from the crate
      uint32_t readLong( unsigned int N, unsigned int A, unsigned int F );
      /// Read 24-bits of information from the crate with the Q and X lines
      uint32_t readLong( unsigned int N, unsigned int A, unsigned int F,
                         bool& Q, bool& X );

      /// Write 16-bits of information to the crate
      void writeWord( unsigned int N, unsigned int A, unsigned int F,
                      uint16_t data );
      /// Write 24-bits of information to the crate
      void writeLong( unsigned int N, unsigned int A, unsigned int F,
                      uint32_t data );

      /// Enable interrupts generated by LAM signals
      bool enableInterrupt();
      /// Disable interrupts generated by LAM signals
      bool disableInterrupt();

      /// Set inhibit signal in CAMAC crate
      void setInhibit();
      /// Reset inhibit signal in CAMAC crate
      void resetInhibit();

      /// Initialize the CAMAC crate (CAMAC Z command)
      void initialize();
      /// Clear the CAMAC crate (CAMAC C command)
      void clear();

      /// Set the LAM mask in the crate controller
      void setLAMMask( uint32_t mask );
      /// Block execution until a LAM signal arrives
      bool waitForLAM();
      /// Clear the LAM flip-flop in the crate controller
      void clearLAM();

   private:
      /// Check if the crate is already open
      bool checkOpen() const;

      const char* m_devicePath; ///< Path to the CAMAC device
#ifndef TESTING
      CC32_HANDLE m_handle;     ///< Handle used by the pcicc32 library
#endif
      bool        m_isOpen;     ///< Flag showing if the crate is open

      mutable msg::Logger m_logger; ///< Object for logging messages

   }; // class Crate

} // namespace camac

#endif // CDA_CORE_CAMAC_CRATE_H
