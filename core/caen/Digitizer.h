// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_COMMON_CAEN_DIGITIZER_H
#define CDA_COMMON_CAEN_DIGITIZER_H

// System include(s):
extern "C" {
#   include <stdint.h>
}

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "msg/Logger.h"

/**
 *  @short Namespace for the CAEN library related classes
 *
 *         Probably this is going to be the only place to use
 *         this namespace. But nevertheless, the CAEN class(es)
 *         should be put in it's/their own namespace.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace caen {

   /**
    * @short Interface to the CAEN Digitizer library
    *
    *        The CAEN XXXX device is accessed using the CAEN
    *        Digitizer library. This class is just used to insulate
    *        the rest of the CDA code from the CAEN libraries.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Digitizer {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( caen::Digitizer )

   public:
      /// Default constructor
      Digitizer();
      /// Destructor
      ~Digitizer();

      /// Enumeration for the connection type to use
      enum ConnectionType {
         USB = 0, ///< USB connection (Only one really supported)
         PCI_OpticalLink = 1,
         PCIE_OpticalLink = 2,
         PCIE_EmbeddedDigitizer = 3
      };

      /// Open the connection
      bool open( ConnectionType type = USB );
      /// Close the connection
      bool close();

      /// Prints information about the digitizer
      bool printInfo() const;

      /// Resets the digitizer
      bool reset();

      /// Clears the data from the digitizer
      bool clear();

      /// Start the data acquisition by the digitizer
      bool startAcquisition();
      /// Stop the data acquisition by the digitizer
      bool stopAcquisition();

      /// Sets up the interrupt mode of the digitizer
      bool setInterruptAfterEvents( uint16_t event_number );
      /// Retrieves the interrupt config of the digitizer
      bool printInterruptConfig() const;
      /// Waits for an interrupt from the digitizer
      bool irqWait( uint32_t timeout ) const;

   private:
      int m_handle; ///< C-style device handle
      /// Number of events after which to send interrupt
      uint16_t m_interruptAfterEvents;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Digitizer

} // namespace caen

#endif // CDA_COMMON_CAEN_DIGITIZER_H
