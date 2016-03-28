// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GROUPGUI_H
#define CDA_DEVICES_DT5740_GROUPGUI_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

// CDA include(s):
#include "msg/Logger.h"

// Local include(s):
#include "GroupConfig.h"
#include "ChannelGui.h"

namespace dt5740 {

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
      GroupGui( GroupConfig& group, QWidget* parent = 0,
                Qt::WindowFlags flags = 0 );

      /// Total width of the widget
      static const int WIDTH;
      /// Total height of the widget
      static const int HEIGHT;

      /// Access one of the channels of the group
      ChannelGui* getChannel( int index ) const;

      /// Function "syncing" to the config object
      void sync();

   private slots:
      /// Slot handling trigger state changes
      void trigEnabledSlot( bool checked );
      /// Slot handling trigger out state changes
      void trigOutEnabledSlot( bool checked );
      /// Slot handling trigger threshold changes
      void trigThresholdSlot( int value );
      /// Slot handling trigger mask changes
      void trigMaskSlot( bool checked );
      /// Slot handling DC offset value changes
      void dcOffsetSlot( int value );

      /// Slot handling changes to channel enablement
      void channelEnabledSlot( int channel, bool on );
      /// Slot handling changes to the channel's raw name
      void rawNameChangedSlot( int channel, const QString& text );

      /// Slot handling changes to time channel names
      void timeNameChangedSlot( int channel, const QString& text );
      /// Slot handling changes to time histogram channel numbers
      void timeChannelsChangedSlot( int channel, int channels );
      /// Slot handling changes to time histogram lower bounds
      void timeLowerBoundChangedSlot( int channel, double value );
      /// Slot handling changes to time histogram upper bounds
      void timeUpperBoundChangedSlot( int channel, double value );

      /// Slot handling changes to energy channel names
      void energyNameChangedSlot( int channel, const QString& text );
      /// Slot handling changes to energy histogram channel numbers
      void energyChannelsChangedSlot( int channel, int channels );
      /// Slot handling changes to energy histogram lower bounds
      void energyLowerBoundChangedSlot( int channel, double value );
      /// Slot handling changes to energy histogram upper bounds
      void energyUpperBoundChangedSlot( int channel, double value );

   private:
      /// Reference to the group that this widget configures
      GroupConfig& m_group;
      bool m_syncing; ///< Flag showing when a syncing operation is going on

      std::unique_ptr< QGroupBox > m_groupBox;

      std::unique_ptr< QCheckBox > m_trigEnabled;
      std::unique_ptr< QCheckBox > m_trigOutEnabled;

      std::unique_ptr< QLabel >   m_trigThresholdLabel;
      std::unique_ptr< QSpinBox > m_trigThreshold;

      std::unique_ptr< QGroupBox > m_trigMaskBox;
      std::unique_ptr< QCheckBox > m_trigMask[ GroupConfig::CHANNELS_IN_GROUP ];

      std::unique_ptr< QLabel >   m_dcOffsetLabel;
      std::unique_ptr< QSpinBox > m_dcOffset;

      /// Label above channel names
      std::unique_ptr< QLabel > m_nameLabel;
      /// Label above histogram channels
      std::unique_ptr< QLabel > m_channelsLabel;
      /// Label above histogram lower bounds
      std::unique_ptr< QLabel > m_lowerBoundLabel;
      /// Label above histogram upper bounds
      std::unique_ptr< QLabel > m_upperBoundLabel;

      /// Graphical representation of channels in the group
      std::unique_ptr< ChannelGui >
      m_channels[ GroupConfig::CHANNELS_IN_GROUP ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class GroupGui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GROUPGUI_H
