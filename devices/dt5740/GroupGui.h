// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_GROUPGUI_H
#define CDA_DEVICES_DT5740_GROUPGUI_H

// Qt include(s):
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#   include "cdacore/common/UniquePtr.h"
#else
#   include "msg/Logger.h"
#   include "common/UniquePtr.h"
#endif

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

      UniquePtr< QGroupBox >::Type m_groupBox;

      UniquePtr< QCheckBox >::Type m_trigEnabled;
      UniquePtr< QCheckBox >::Type m_trigOutEnabled;

      UniquePtr< QLabel >::Type   m_trigThresholdLabel;
      UniquePtr< QSpinBox >::Type m_trigThreshold;

      UniquePtr< QGroupBox >::Type m_trigMaskBox;
      UniquePtr< QCheckBox >::Type m_trigMask[ GroupConfig::CHANNELS_IN_GROUP ];

      UniquePtr< QLabel >::Type   m_dcOffsetLabel;
      UniquePtr< QSpinBox >::Type m_dcOffset;

      /// Label above channel names
      UniquePtr< QLabel >::Type m_nameLabel;
      /// Label above histogram channels
      UniquePtr< QLabel >::Type m_channelsLabel;
      /// Label above histogram lower bounds
      UniquePtr< QLabel >::Type m_lowerBoundLabel;
      /// Label above histogram upper bounds
      UniquePtr< QLabel >::Type m_upperBoundLabel;

      /// Graphical representation of channels in the group
      UniquePtr< ChannelGui >::Type
      m_channels[ GroupConfig::CHANNELS_IN_GROUP ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class GroupGui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_GROUPGUI_H
