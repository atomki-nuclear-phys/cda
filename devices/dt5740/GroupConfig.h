// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GROUPCONFIG_H
#define CDA_DEVICES_DT5740_GROUPCONFIG_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Config.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Config.h"
#   include "msg/Logger.h"
#endif

namespace dt5740 {

   // Forward declaration(s):
   class ChannelConfig;

   /**
    *  @short Configuration for a single channel group
    *
    *         The input channels of the DT5740 are divided into
    *         4 groups, with 8 channels each. Much of the configuration
    *         for the channels can be set on the group level. This
    *         class stores the configuration options of one of these
    *         groups.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class GroupConfig : virtual public dev::Config {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( dt5740::GroupConfig )

   public:
      /// Default constructor
      GroupConfig();
      /// Copy constructor
      GroupConfig( const GroupConfig& parent );
      /// Destructor
      ~GroupConfig();

      /// Assignment operator
      GroupConfig& operator= ( const GroupConfig& rh );

      /// Number of channels belonging to a channel group
      static const int CHANNELS_IN_GROUP = 8;

      /// Function reading the configuration in binary format
      virtual bool readConfig( QIODevice* dev );
      /// Function writing the configuration in binary format
      virtual bool writeConfig( QIODevice* dev ) const;

      /// Function reading the configuration in XML format
      virtual bool readConfig( const QDomElement& node );
      /// Function writing the configuration in XML format
      virtual bool writeConfig( QDomElement& node ) const;

      /// Triggering mode types
      enum TriggerMode {
         /// Generate trigger when a channel goes over the threshold
         TriggerOnInputOverThreshold  = 0,
         /// Generate trigger when a channel goes under the threshold
         TriggerOnInputUnderThreshold = 1
      }; // enum TriggerMode

      enum GateMode {
         WindowGate     = 0,
         SingleShotGate = 1
      }; // enum GateMode

      /// Buffer organization mode
      enum BufferMode {
         NBuffers1    = 0, ///< 1 buffer with 192k samples
         NBuffers2    = 1, ///< 2 buffers with 96k samples
         NBuffers4    = 2, ///< 4 buffers with 48k samples
         NBuffers8    = 3, ///< 8 buffers with 24k samples
         NBuffers16   = 4, ///< 16 buffers with 12k samples
         NBuffers32   = 5, ///< 32 buffers with 6k samples
         NBuffers64   = 6, ///< 64 buffers with 3k samples
         NBuffers128  = 7, ///< 128 buffers with 1536 samples
         NBuffers256  = 8, ///< 256 buffers with 768 samples
         NBuffers512  = 9, ///< 512 buffers with 384 samples
         NBuffers1024 = 10 ///< 1024 buffers with 192 samples
      }; // enum BufferMode

      /// Get the number of this group
      int getGroupNumber() const;
      /// Set the number of this group
      void setGroupNumber( int value );

      /// Get if self-triggering is enabled
      bool getTrigEnabled() const;
      /// Set if self-triggering is enabled
      void setTrigEnabled( bool on );

      /// Get if trigger output to the front pane is enabled
      bool getTrigOutEnabled() const;
      /// Set if trigger output to the front pane is enabled
      void setTrigOutEnabled( bool on );

      /// Get if overlapping triggers are enabled
      bool getTrigOvlpEnabled() const;
      /// Set if overlapping triggers are enabled
      void setTrigOvlpEnabled( bool on );

      /// Get the trigger mode of the group
      TriggerMode getTrigMode() const;
      /// Set the trigger mode of the group
      void setTrigMode( TriggerMode mode );

      /// Get the trigger threshold
      int getTrigThreshold() const;
      /// Set the trigger threshold
      void setTrigThreshold( int value );

      /// Get the trigger mask for the channels
      unsigned int getTrigMask() const;
      /// Set the trigger mask for the channels
      void setTrigMask( unsigned int mask );

      /// Get the percentage of post trigger samples to collect
      int getPostTrigPercentage() const;
      /// Set the percentage of post trigger samples to collect
      void setPostTrigPercentage( int value );

      /// Get the DC offset for the group
      int getDCOffset() const;
      /// Set the DC offset for the group
      void setDCOffset( int value );

      /// Get if test pattern generation is enabled
      bool getPatGenEnabled() const;
      /// Set if test pattern generation is enabled
      void setPatGenEnabled( bool on );

      /// Get the gate mode of the group
      GateMode getGateMode() const;
      /// Set the gate mode of the troup
      void setGateMode( GateMode mode );

      /// Get the buffer mode of the group
      BufferMode getBufferMode() const;
      /// Set the buffer mode of the group
      void setBufferMode( BufferMode mode );
      /// Helper function for getting the number of samples
      int getSamples() const;

      /// Enable/disable one of the channels
      void enableChannel( int channel, bool enable );
      /// Access one of the channels
      ChannelConfig* getChannel( int channel );
      /// Access one of the channels (constant version)
      const ChannelConfig* getChannel( int channel ) const;

      /// Clear the group's configuration
      void clear();

   private:
      /// Transform trigger mode into an integer
      unsigned int toUInt( TriggerMode mode ) const;
      /// Transform gate mode into an integer
      unsigned int toUInt( GateMode mode ) const;
      /// Transform buffer mode into an integer
      unsigned int toUInt( BufferMode mode ) const;
      /// Create a trigger mode from an integer
      TriggerMode toTriggerMode( unsigned int value ) const;
      /// Create a gate mode from an integer
      GateMode toGateMode( unsigned int value ) const;
      /// Create a buffer mode from an integer
      BufferMode toBufferMode( unsigned int value ) const;

      int m_groupNumber; ///< Group number (0..3)

      int          m_trigThreshold; ///< Trigger threshold for the group
      int          m_dcOffset; ///< DC offset for the group
      unsigned int m_trigMask; ///< Mask for enabling channels to trigger
      TriggerMode  m_trigMode; ///< Trigger mode for the group
      bool         m_trigOvlpEnabled; ///< Enable triggers to overlap
      bool         m_patGenEnabled; ///< Enable test pattern generation
      GateMode     m_gateMode; ///< Don't quite understand this one...
      BufferMode   m_bufferMode; ///< Division mode of the event buffer
      bool         m_trigEnabled; ///< Enable triggering on this group
      bool         m_trigOutEnabled; ///< Enable to send triggers to front panel
      int          m_postTrigPercentage; ///< Percentage of post-trigger samples

      /// Configuration of the input channels belonging to this group
      ChannelConfig* m_channels[ CHANNELS_IN_GROUP ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class GroupConfig

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GROUPCONFIG_H
