//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2025 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_COMMON_CAEN_DIGITIZER_H
#define CDA_COMMON_CAEN_DIGITIZER_H

// System include(s):
extern "C" {
#include <stdint.h>
}
#include <csignal>

// Qt include(s):
#include <QCoreApplication>

// CDA include(s):
#include "common/Export.h"
#include "common/StatusCode.h"
#include "msg/Logger.h"

/**
 *  @short Namespace for the CAEN library related classes
 *
 *         Probably this is going to be the only place to use
 *         this namespace. But nevertheless, the CAEN class(es)
 *         should be put in it's/their own namespace.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
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
 */
class CDACORE_EXPORT Digitizer {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(caen::Digitizer)

public:
   /// Default constructor
   Digitizer();
   /// Destructor
   ~Digitizer();

   /// Enumeration for the connection type to use
   enum ConnectionType {
      USB = 0,             ///< USB connection (direct or V1718->VME)
      PCI_OpticalLink = 1  ///< PCI-A2818 (direct or V2718->VME)
   };

   /// Helper function converting connection type
   static int convertConnType(ConnectionType type);
   /// Helper function converting connection type
   static ConnectionType convertConnType(int type);

   /// Open the connection
   StatusCode open(ConnectionType type = USB, int linkNum = 0,
                   int conetNode = 0, uint32_t vmeAddress = 0);
   /// Close the connection
   StatusCode close();

   /// Write to a 32-bit register
   StatusCode writeRegister(uint32_t address, uint32_t data);
   /// Read from a 32-bit register
   StatusCode readRegister(uint32_t address, uint32_t& data) const;

   /// Prints information about the digitizer
   StatusCode printInfo(msg::Level level) const;

   /// Resets the digitizer
   StatusCode reset();

   /// Clears the data from the digitizer
   StatusCode clear();

   /// Acquisition mode
   enum AcquisitionMode {
      ACQ_SW_Controlled = 0,  ///< Acquisition starts on SW command
      ACQ_SIn_Controlled = 1  ///< Acquisition starts on SIn signal
   };

   /// Helper function converting the acquisition mode
   static int convertAcqMode(AcquisitionMode mode);
   /// Helper function converting the acquisition mode
   static AcquisitionMode convertAcqMode(int mode);

   /// Start the data acquisition by the digitizer
   StatusCode startAcquisition();
   /// Stop the data acquisition by the digitizer
   StatusCode stopAcquisition();
   /// Set the acquisition mode
   StatusCode setAcquisitionMode(AcquisitionMode mode);
   /// Get the acquisition mode
   StatusCode getAcquisitionMode(AcquisitionMode& mode) const;

   /// Sets up the interrupt mode of the digitizer
   StatusCode setInterruptAfterEvents(uint16_t event_number);
   /// Retrieves the interrupt config of the digitizer
   StatusCode printInterruptConfig() const;
   /// Waits for an interrupt from the digitizer
   StatusCode irqWait(uint32_t timeout) const;

   /// Enumeration listing the channel trigger modes
   enum TriggerMode {
      TRIG_Disabled = 0,  ///< Triggering is disabled
      TRIG_ExtOnly = 1,   ///< Trigger only for external output
      TRIG_AcqOnly = 2,   ///< Trigger only according for data acquisition
      TRIG_AcqAndExt = 3  ///< Send trigger for both purposes
   };

   /// Sets the SW triggering mode
   StatusCode setSWTriggerMode(TriggerMode mode);
   /// Retrieves the SW triggering mode
   StatusCode getSWTriggerMode(TriggerMode& mode) const;
   /// Sets the external triggering mode
   StatusCode setExtTriggerMode(TriggerMode mode);
   /// Retrieves the external triggering mode
   StatusCode getExtTriggerMode(TriggerMode& mode) const;
   /// Sets the triggering mode for one channel
   StatusCode setChannelSelfTriggerMode(uint32_t channel, TriggerMode mode);
   /// Retrieves the triggering mode of one channel
   StatusCode getChannelSelfTriggerMode(uint32_t channel,
                                        TriggerMode& mode) const;
   /// Sets the triggering mode for one group
   StatusCode setGroupSelfTriggerMode(uint32_t group, TriggerMode mode);
   /// Retrieves the triggering mode of one group
   StatusCode getGroupSelfTriggerMode(uint32_t group, TriggerMode& mode) const;

   /// Send a software trigger to the device
   StatusCode sendSWTrigger();

   /// Sets which channels of a group should generate self triggers
   StatusCode setChannelGroupMask(uint32_t group, uint32_t mask);
   /// Gets which channels of a group should generate self triggers
   StatusCode getChannelGroupMask(uint32_t group, uint32_t& mask) const;

   /// Sets the trigger threshold for a specific channel
   StatusCode setChannelTriggerThreshold(uint32_t channel, uint32_t thr);
   /// Retrieves the trigger threshold of a specific channel
   StatusCode getChannelTriggerThreshold(uint32_t channel, uint32_t& thr) const;
   /// Sets the trigger threshold for a group
   StatusCode setGroupTriggerThreshold(uint32_t group, uint32_t thr);
   /// Retrieves the trigger threshold of a group
   StatusCode getGroupTriggerThreshold(uint32_t group, uint32_t& thr) const;

   /// Sets which channels are enabled for readout
   StatusCode setChannelEnableMask(uint32_t mask);
   /// Gets which channels are enabled for readout
   StatusCode getChannelEnableMask(uint32_t& mask) const;
   /// Sets which groups are enabled for readout
   StatusCode setGroupEnableMask(uint32_t mask);
   /// Gets which groups are enabled for readout
   StatusCode getGroupEnableMask(uint32_t& mask) const;

   /// Sets the record length of the digitizer
   StatusCode setRecordLength(uint32_t size);
   /// Gets the record length of the digitizer
   StatusCode getRecordLength(uint32_t& size) const;
   /// Sets how much of the readout is after the trigger
   StatusCode setPostTriggerSize(uint32_t percent);
   /// Gets how much of the readout is after the trigger
   StatusCode getPostTriggerSize(uint32_t& percent) const;

   /// Sets the DC offset for a specified channel
   StatusCode setChannelDCOffset(uint32_t channel, uint32_t value);
   /// Retrieves the DC offset of a specified channel
   StatusCode getChannelDCOffset(uint32_t channel, uint32_t& value) const;
   /// Sets the DC offset for a group
   StatusCode setGroupDCOffset(uint32_t group, uint32_t value);
   /// Retrieves the DC offset of a group
   StatusCode getGroupDCOffset(uint32_t group, uint32_t& value) const;

   /// Sets the maximum number of events during readout
   StatusCode setMaxNumEventsBLT(uint32_t events);
   /// Retrieves the maximum number of events during readout
   StatusCode getMaxNumEventsBLT(uint32_t& events) const;

   /// Data reading modes
   enum ReadMode {
      READ_SlaveTerminatedMBLT = 0,
      READ_SlaveTerminated2eVME = 1,
      READ_SlaveTerminated2eSST = 2,
      READ_PollingMBLT = 3,
      READ_Polling2eVME = 4,
      READ_Polling2eSST = 5
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
      std::vector<uint16_t> chData[MAX_CHANNEL_NUMBER];
   };
   /// 8 bit event data
   struct EventData8Bit {
      static const int MAX_CHANNEL_NUMBER = 8;
      std::vector<uint8_t> chData[MAX_CHANNEL_NUMBER];
   };

   /// Allocates the readout buffer in memory
   StatusCode mallocReadoutBuffer(char** buffer, uint32_t& size);
   /// Frees the memory allocated for the readout buffer
   StatusCode freeReadoutBuffer(char** buffer);
   /// Read data from the digitizer
   StatusCode readData(ReadMode mode, char* buffer, uint32_t& bufferSize);
   /// Get the number of events in the readout buffer
   StatusCode getNumEvents(char* buffer, uint32_t bufferSize,
                           uint32_t& numEvents) const;
   /// Get the data of one of the events in the readout buffer
   StatusCode getEvent(char* buffer, uint32_t bufferSize, int32_t event,
                       EventInfo& eventInfo, EventData16Bit& eventData);
   /// Get the data of one of the events in the readout buffer
   StatusCode getEvent(char* buffer, uint32_t bufferSize, int32_t event,
                       EventInfo& eventInfo, EventData8Bit& eventData);

private:
   int m_handle;  ///< C-style device handle

#ifndef HAVE_CAEN_DIGITIZER_LIBS
   uint32_t m_recordLength;  ///< Record length to read out in test mode
#endif                       // HAVE_CAEN_DIGITIZER_LIBS

#ifndef Q_OS_WIN
   sigset_t
       m_blockedSignals;  ///< Signals to block during hardware communication
#endif                    // Q_OS_WIN

   mutable msg::Logger m_logger;  ///< Message logger object

};  // class Digitizer

}  // namespace caen

#endif  // CDA_COMMON_CAEN_DIGITIZER_H
