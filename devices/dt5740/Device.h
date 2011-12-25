// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_DEVICE_H
#define CDA_DEVICES_DT5740_DEVICE_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Device.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Device.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "ChannelConfig.h"
#include "GroupConfig.h"

// Forward declaration(s):
namespace ev {
   class Fragment;
}

namespace dt5740 {

   /**
    *  @short Base class for the DT5740 devices
    *
    *         This class stores the configuration of a DT5740 digitizer
    *         and takes care of reading and writing this configuration
    *         in all the necessary formats.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Device : virtual public dev::Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dt5740::Device )

   public:
      /// Constructor
      Device();
      /// Destructor
      ~Device();

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice* dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

      /// The type of the child class
      virtual QString deviceName() const { return "DT5740"; }

      /// ID of the USB connection
      virtual unsigned int getID() const;
      /// Sets the ID of the USB connection (not used)
      virtual void setID( unsigned int value );

   protected:
      /// Triggering mode types
      enum TriggerMode {
         /// Generate trigger when a channel goes over the threshold
         TRG_InputOverThreshold  = 0,
         /// Generate trigger when a channel goes under the threshold
         TRG_InputUnderThreshold = 1
      }; // enum TriggerMode

      /// Really not sure what this is...
      enum GateMode {
         GATE_Window     = 0,
         GATE_SingleShot = 1
      }; // enum GateMode

      /// Buffer organization mode
      enum BufferMode {
         BUFF_NBuffers1    = 0, ///< 1 buffer with 192k samples
         BUFF_NBuffers2    = 1, ///< 2 buffers with 96k samples
         BUFF_NBuffers4    = 2, ///< 4 buffers with 48k samples
         BUFF_NBuffers8    = 3, ///< 8 buffers with 24k samples
         BUFF_NBuffers16   = 4, ///< 16 buffers with 12k samples
         BUFF_NBuffers32   = 5, ///< 32 buffers with 6k samples
         BUFF_NBuffers64   = 6, ///< 64 buffers with 3k samples
         BUFF_NBuffers128  = 7, ///< 128 buffers with 1536 samples
         BUFF_NBuffers256  = 8, ///< 256 buffers with 768 samples
         BUFF_NBuffers512  = 9, ///< 512 buffers with 384 samples
         BUFF_NBuffers1024 = 10 ///< 1024 buffers with 192 samples
      }; // enum BufferMode

      /// Clock source
      enum ClockSource {
         CLK_Internal = 0, ///< Use the internal clock
         CLK_External = 1  ///< Use an external clock
      }; // enum Clock

      /// Event counting method
      enum EvCountMode {
         EV_CountAcceptedTriggers = 0, ///< Only count the accepted triggers
         EV_CountAllTriggers = 1       ///< Count all received triggers
      }; // enum EvCountMode

      /// Front panel signal standard
      enum SignalType {
         SGNL_NIM = 0, ///< The front panel expects/provides NIM signals
         SGNL_TTL = 1  ///< The front panel expects/provides TTL signals
      }; // enum SignalStandard

      /// Clear the configuration of the device
      void clear();
      /// Helper function for getting the number of samples
      int getSamples() const;

      /// Function decoding the data from an event fragment
      bool decode( const ev::Fragment& fragment,
                   caen::Digitizer::EventInfo& ei,
                   caen::Digitizer::EventData16Bit& ed ) const;
      /// Function encoding the data into an event fragment
      bool encode( const caen::Digitizer::EventInfo& ei,
                   const caen::Digitizer::EventData16Bit& ed,
                   ev::Fragment& fragment ) const;

      /// Transform trigger mode into a string
      QString toString( TriggerMode mode ) const;
      /// Transform gate mode into a string
      QString toString( GateMode mode ) const;
      /// Transform buffer mode into a string
      QString toString( BufferMode mode ) const;
      /// Transform the clock source into a string
      QString toString( ClockSource source ) const;
      /// Transform event counting mode into a string
      QString toString( EvCountMode mode ) const;
      /// Transform the front panel signal type into a string
      QString toString( SignalType type ) const;
      /// Create a trigger mode from a string
      TriggerMode toTriggerMode( const QString& value ) const;
      /// Create a gate mode from a string
      GateMode toGateMode( const QString& value ) const;
      /// Create a buffer mode from a string
      BufferMode toBufferMode( const QString& value ) const;
      /// Create a clock source from a string
      ClockSource toClockSource( const QString& value ) const;
      /// Create an event counting mode from a string
      EvCountMode toEvCountMode( const QString& value ) const;
      /// Create a signal type from a string
      SignalType toSignalType( const QString& value ) const;

      /// Number of channel groups handled by the device
      static const int NUMBER_OF_GROUPS = 4;
      /// Configuration of the channel groups
      GroupConfig m_groups[ NUMBER_OF_GROUPS ];

      /// Connection protocol to access the device
      caen::Digitizer::ConnectionType m_connType;
      /// Link number (0 if only USB device)
      int m_linkNumber;

      TriggerMode m_trigMode; ///< Trigger mode
      bool        m_trigOvlpEnabled; ///< Enable triggers to overlap
      bool        m_patGenEnabled; ///< Enable test pattern generation
      GateMode    m_gateMode; ///< Don't quite understand this one...
      BufferMode  m_bufferMode; ///< Division mode of the event buffer
      int         m_postTrigPercentage; ///< Percentage of post-trigger samples
      bool        m_extTrigEnabled; ///< Enable readout on external triggers
      bool        m_extTrigOutEnabled; ///< Forward external triggers to front panel
      ClockSource m_clockSource; ///< Clock source to use in the digitization
      EvCountMode m_evCountMode; ///< Event counting mode in the device
      SignalType  m_signalType; ///< Signal type on the front panel
      bool        m_highImpedanceGPO; ///< Output connector should have high impedance?

      /// Acquisition mode for the device
      caen::Digitizer::AcquisitionMode m_acqMode;

      /// Flag showing whether raw information should be put into ntuples
      bool m_saveRawNtuple;

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Device

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_DEVICE_H
