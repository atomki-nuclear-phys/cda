// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GROUPGUI_H
#define CDA_DEVICES_DT5740_GROUPGUI_H

// Qt include(s):
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "GroupConfig.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QComboBox )
QT_FORWARD_DECLARE_CLASS( QSpinBox )

namespace dt5740 {

   // Forward declaration(s):
   class ChannelGui;

   /**
    *  @short Graphical representation of a channel group
    *
    *         This class is used to configure a single channel group
    *         in the DT5740 digitizer.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class GroupGui : public QWidget {

      Q_OBJECT

   public:
      /// Constructor
      GroupGui( int group, QWidget* parent = 0,
                Qt::WindowFlags flags = 0 );
      /// Destructor
      ~GroupGui();

      /// Get the group described by this widget
      int getGroupNumber() const;

      /// Total width of the widget
      static const int WIDTH;
      /// Total height of the widget
      static const int HEIGHT;

      /// Access one of the channels of the group
      ChannelGui* getChannel( int index ) const;

      /// Function "syncing" to the config object
      void sync( const GroupConfig& conf );

   signals:
      /// Signal emitted when the self triggering state changes
      void trigEnabled( int group, bool state );
      /// Signal emitted when the trigger output state changes
      void trigOutEnabled( int group, bool state );
      /// Signal emitted when the trigger overlap state changes
      void trigOvlpEnabled( int group, bool state );
      /// Signal emitted when the trigger mode changes
      void trigMode( int group, dt5740::GroupConfig::TriggerMode mode );
      /// Signal emitted when the trigger threshold changes
      void trigThreshold( int group, int value );
      /// Signal emitted when the trigger mask changes
      void trigMask( int group, unsigned int value );
      /// Signal emitted when the post trigger percentage value changes
      void postTrigPercentage( int group, int value );
      /// Signal emitted when the DC offset changes
      void dcOffset( int group, int value );
      /// Signal emitted when the pattern generation state changes
      void patGenEnabled( int group, bool state );
      /// Signal emitted when the gate mode changes
      void gateMode( int group, dt5740::GroupConfig::GateMode mode );
      /// Signal emitted when the buffer mode changes
      void bufferMode( int group, dt5740::GroupConfig::BufferMode mode );

   private slots:
      /// Slot handling trigger state changes
      void trigEnabledSlot( bool checked );
      /// Slot handling trigger out state changes
      void trigOutEnabledSlot( bool checked );
      /// Slot handling trigger overlap state changes
      void trigOvlpEnabledSlot( bool checked );
      /// Slot handling trigger mode changes
      void trigModeSlot( int index );
      /// Slot handling trigger threshold changes
      void trigThresholdSlot( int value );
      /// Slot handling trigger mask changes
      void trigMaskSlot( bool checked );
      /// Slot handling post trigger percentage changes
      void postTrigPercentageSlot( int value );
      /// Slot handling DC offset value changes
      void dcOffsetSlot( int value );
      /// Slot handling changes to the pattern generation state
      void patGenEnabledSlot( bool checked );
      /// Slot handling the gate mode changes
      void gateModeSlot( int index );
      /// Slot handling the buffer mode changes
      void bufferModeSlot( int index );

   private:
      const int m_groupNumber; ///< Channel group ID
      bool m_syncing; ///< Flag showing when a syncing operation is going on

      QGroupBox* m_groupBox;

      QCheckBox* m_trigEnabled;
      QCheckBox* m_trigOutEnabled;
      QCheckBox* m_trigOvlpEnabled;

      QLabel*    m_trigModeLabel;
      QComboBox* m_trigMode;

      QLabel*   m_trigThresholdLabel;
      QSpinBox* m_trigThreshold;

      QGroupBox* m_trigMaskBox;
      QCheckBox* m_trigMask[ GroupConfig::CHANNELS_IN_GROUP ];

      QLabel*   m_postTrigPercentageLabel;
      QSpinBox* m_postTrigPercentage;

      QLabel*   m_dcOffsetLabel;
      QSpinBox* m_dcOffset;

      QCheckBox* m_patGenEnabled;

      QLabel*    m_gateModeLabel;
      QComboBox* m_gateMode;

      QLabel*    m_bufferModeLabel;
      QComboBox* m_bufferMode;

      QLabel* m_nameLabel; ///< Label above channel names
      QLabel* m_channelsLabel; ///< Label above histogram channels
      QLabel* m_lowerBoundLabel; ///< Label above histogram lower bounds
      QLabel* m_upperBoundLabel; ///< Label above histogram upper bounds

      /// Graphical representation of channels in the group
      ChannelGui* m_channels[ GroupConfig::CHANNELS_IN_GROUP ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class GroupGui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GROUPGUI_H
