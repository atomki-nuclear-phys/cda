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
#include "../msg/Logger.h"

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

      /// Write to a 32-bit register
      bool writeRegister( uint32_t address, uint32_t data );
      /// Read from a 32-bit register
      bool readRegister( uint32_t address, uint32_t& data ) const;

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

      /// Set which channels should be enabled
      bool setChannelEnableMask( uint32_t mask );
      /// Retrieves which channels are enabled
      bool getChannelEnableMask( uint32_t& mask ) const;

      /// Enumeration listing the channel trigger modes
      enum TriggerMode {
         TRIG_Disabled  = 0, ///< Triggering for the channel is disabled
         TRIG_ExtOnly   = 1, ///< Trigger only on external input
         TRIG_AcqOnly   = 2, ///< Trigger only according to data acquisition
         TRIG_AcqAndExt = 3  ///< Trigger on both inputs
      };

      /// Sets the global triggering mode
      bool setGlobalTriggerMode( TriggerMode mode );
      /// Retrieves the global triggering mode
      bool getGlobalTriggerMode( TriggerMode& mode ) const;
      /// Sets the triggering mode for one channel
      bool setChannelTriggerMode( uint32_t channel, TriggerMode mode );
      /// Retrieves the triggering mode of one channel
      bool getChannelTriggerMode( uint32_t channel,
                                  TriggerMode& mode ) const;

      /// Sets the trigger threshold for a specific channel
      bool setChannelTriggerThreshold( uint32_t channel, uint32_t thr );
      /// Retrieves the trigger threshold of a specific channel
      bool getChannelTriggerThreshold( uint32_t channel,
                                       uint32_t& thr ) const;

      /// Sets the DC offset for a specified channel
      bool setDCOffset( uint32_t channel, uint32_t value );
      /// Retrieves the DC offset of a specified channel
      bool getDCOffset( uint32_t channel, uint32_t& value ) const;

   private:
      int m_handle; ///< C-style device handle
      /// Number of events after which to send interrupt
      uint16_t m_interruptAfterEvents;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Digitizer

} // namespace caen

#endif // CDA_COMMON_CAEN_DIGITIZER_H
