// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_COMMON_CAEN_DIGITIZER_H
#define CDA_COMMON_CAEN_DIGITIZER_H

// System include(s):
extern "C" {
#   include <stdint.h>
}
#include <csignal>

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
         USB = 0, ///< USB connection (direct or V1718->VME)
         PCI_OpticalLink = 1, ///< PCI-A2818 (direct or V2718->VME)
         PCIE_OpticalLink = 2, ///< PCIe-A3818 (direct or A3818->VME)
         PCIE_EmbeddedDigitizer = 3 ///< Possible future development
      };

      /// Helper function converting connection type
      static int convertConnType( ConnectionType type );
      /// Helper function converting connection type
      static ConnectionType convertConnType( int type );

      /// Open the connection
      bool open( ConnectionType type = USB, int linkNum = 0,
                 int conetNode = 0, uint32_t vmeAddress = 0 );
      /// Close the connection
      bool close();

      /// Write to a 32-bit register
      bool writeRegister( uint32_t address, uint32_t data );
      /// Read from a 32-bit register
      bool readRegister( uint32_t address, uint32_t& data ) const;

      /// Prints information about the digitizer
      bool printInfo( msg::Level level ) const;

      /// Resets the digitizer
      bool reset();

      /// Clears the data from the digitizer
      bool clear();

      /// Acquisition mode
      enum AcquisitionMode {
         ACQ_SW_Controlled  = 0, ///< Acquisition starts on SW command
         ACQ_SIn_Controlled = 1  ///< Acquisition starts on SIn signal
      };

      /// Helper function converting the acquisition mode
      static int convertAcqMode( AcquisitionMode mode );
      /// Helper function converting the acquisition mode
      static AcquisitionMode convertAcqMode( int mode );

      /// Start the data acquisition by the digitizer
      bool startAcquisition();
      /// Stop the data acquisition by the digitizer
      bool stopAcquisition();
      /// Set the acquisition mode
      bool setAcquisitionMode( AcquisitionMode mode );
      /// Get the acquisition mode
      bool getAcquisitionMode( AcquisitionMode& mode ) const;

      /// Sets up the interrupt mode of the digitizer
      bool setInterruptAfterEvents( uint16_t event_number );
      /// Retrieves the interrupt config of the digitizer
      bool printInterruptConfig() const;
      /// Waits for an interrupt from the digitizer
      bool irqWait( uint32_t timeout ) const;

      /// Enumeration listing the channel trigger modes
      enum TriggerMode {
         TRIG_Disabled  = 0, ///< Triggering is disabled
         TRIG_ExtOnly   = 1, ///< Trigger only for external output
         TRIG_AcqOnly   = 2, ///< Trigger only according for data acquisition
         TRIG_AcqAndExt = 3  ///< Send trigger for both purposes
      };

      /// Sets the SW triggering mode
      bool setSWTriggerMode( TriggerMode mode );
      /// Retrieves the SW triggering mode
      bool getSWTriggerMode( TriggerMode& mode ) const;
      /// Sets the external triggering mode
      bool setExtTriggerMode( TriggerMode mode );
      /// Retrieves the external triggering mode
      bool getExtTriggerMode( TriggerMode& mode ) const;
      /// Sets the triggering mode for one channel
      bool setChannelSelfTriggerMode( uint32_t channel,
                                      TriggerMode mode );
      /// Retrieves the triggering mode of one channel
      bool getChannelSelfTriggerMode( uint32_t channel,
                                      TriggerMode& mode ) const;
      /// Sets the triggering mode for one group
      bool setGroupSelfTriggerMode( uint32_t group,
                                    TriggerMode mode );
      /// Retrieves the triggering mode of one group
      bool getGroupSelfTriggerMode( uint32_t group,
                                    TriggerMode& mode ) const;

      /// Send a software trigger to the device
      bool sendSWTrigger();

      /// Sets which channels of a group should generate self triggers
      bool setChannelGroupMask( uint32_t group,
                                uint32_t mask );
      /// Gets which channels of a group should generate self triggers
      bool getChannelGroupMask( uint32_t group,
                                uint32_t& mask ) const;

      /// Sets the trigger threshold for a specific channel
      bool setChannelTriggerThreshold( uint32_t channel,
                                       uint32_t thr );
      /// Retrieves the trigger threshold of a specific channel
      bool getChannelTriggerThreshold( uint32_t channel,
                                       uint32_t& thr ) const;
      /// Sets the trigger threshold for a group
      bool setGroupTriggerThreshold( uint32_t group,
                                     uint32_t thr );
      /// Retrieves the trigger threshold of a group
      bool getGroupTriggerThreshold( uint32_t group,
                                     uint32_t& thr ) const;

      /// Sets which channels are enabled for readout
      bool setChannelEnableMask( uint32_t mask );
      /// Gets which channels are enabled for readout
      bool getChannelEnableMask( uint32_t& mask ) const;
      /// Sets which groups are enabled for readout
      bool setGroupEnableMask( uint32_t mask );
      /// Gets which groups are enabled for readout
      bool getGroupEnableMask( uint32_t& mask ) const;

      /// Sets the record length of the digitizer
      bool setRecordLength( uint32_t size );
      /// Gets the record length of the digitizer
      bool getRecordLength( uint32_t& size ) const;
      /// Sets how much of the readout is after the trigger
      bool setPostTriggerSize( uint32_t percent );
      /// Gets how much of the readout is after the trigger
      bool getPostTriggerSize( uint32_t& percent ) const;

      /// Sets the DC offset for a specified channel
      bool setChannelDCOffset( uint32_t channel,
                               uint32_t value );
      /// Retrieves the DC offset of a specified channel
      bool getChannelDCOffset( uint32_t channel,
                               uint32_t& value ) const;
      /// Sets the DC offset for a group
      bool setGroupDCOffset( uint32_t group,
                             uint32_t value );
      /// Retrieves the DC offset of a group
      bool getGroupDCOffset( uint32_t group,
                             uint32_t& value ) const;

      /// Sets the maximum number of events during readout
      bool setMaxNumEventsBLT( uint32_t events );
      /// Retrieves the maximum number of events during readout
      bool getMaxNumEventsBLT( uint32_t& events ) const;

      /// Data reading modes
      enum ReadMode {
         READ_SlaveTerminatedMBLT  = 0,
         READ_SlaveTerminated2eVME = 1,
         READ_SlaveTerminated2eSST = 2,
         READ_PollingMBLT          = 3,
         READ_Polling2eVME         = 4,
         READ_Polling2eSST         = 5
      };

      /// Event information
      struct EventInfo {
         uint32_t eventSize;
         uint32_t boardId;
         uint32_t pattern;
         uint32_t channelMask;
         uint32_t eventCounter;
         uint32_t triggerTimeTag;
      };

      /// 16 bit event data
      struct EventData16Bit {
         static const int MAX_CHANNEL_NUMBER = 64;
         std::vector< uint16_t > chData[ MAX_CHANNEL_NUMBER ];
      };
      /// 8 bit event data
      struct EventData8Bit {
         static const int MAX_CHANNEL_NUMBER = 8;
         std::vector< uint8_t > chData[ MAX_CHANNEL_NUMBER ];
      };

      /// Allocates the readout buffer in memory
      bool mallocReadoutBuffer( char** buffer, uint32_t& size );
      /// Frees the memory allocated for the readout buffer
      bool freeReadoutBuffer( char** buffer );
      /// Read data from the digitizer
      bool readData( ReadMode mode, char* buffer,
                     uint32_t& bufferSize );
      /// Get the number of events in the readout buffer
      bool getNumEvents( char* buffer, uint32_t bufferSize,
                         uint32_t& numEvents ) const;
      /// Get the data of one of the events in the readout buffer
      bool getEvent( char* buffer, uint32_t bufferSize,
                     int32_t event, EventInfo& eventInfo,
                     EventData16Bit& eventData );
      /// Get the data of one of the events in the readout buffer
      bool getEvent( char* buffer, uint32_t bufferSize,
                     int32_t event, EventInfo& eventInfo,
                     EventData8Bit& eventData );

   private:
      int m_handle; ///< C-style device handle

#ifndef HAVE_CAEN_LIBS
      uint32_t m_recordLength; ///< Record length to read out in test mode
#endif // HAVE_CAEN_LIBS

#ifndef Q_OS_WIN
      sigset_t m_blockedSignals; ///< Signals to block during hardware communication
#endif // Q_OS_WIN

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Digitizer

} // namespace caen

#endif // CDA_COMMON_CAEN_DIGITIZER_H
