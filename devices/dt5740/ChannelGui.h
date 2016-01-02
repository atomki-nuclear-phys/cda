// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_CHANNELGUI_H
#define CDA_DEVICES_DT5740_CHANNELGUI_H

// Qt include(s):
#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/UniquePtr.h"
#else
#   include "common/UniquePtr.h"
#endif

namespace dt5740 {

   /**
    *  @short Class for modifying the properties of one input channel
    *
    *         Each input channel of the DT5740 has the same properties
    *         that the user would want to change. These are the same
    *         properties that are stored in the dt5740::ChannelConfig
    *         class.
    *
    *         This is a fixed size widget that has a Qt widget for changing
    *         each property of a channel. It looks like this on a Mac:
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class ChannelGui : public QWidget {

      Q_OBJECT

   public:
      /// Constructor
      ChannelGui( int channel, QWidget* parent = 0,
                  Qt::WindowFlags flags = 0 );

      /// Get the subaddress described by this widget
      int getChannelNumber() const;
      /// Set the subaddress described by this widget
      void setChannelNumber( int value );

      /// Total width of the widget
      static const int WIDTH;
      /// Total height of the widget
      static const int HEIGHT;

   signals:
      /// Signal emitted when the channel is turned on or off
      /**
       * @param channel The channel number
       * @param on The state of the object
       */
      void enableChanged( int channel, bool on );
      /// Signal emitted when the raw name of the channel changes
      /**
       * @param channel The channel number
       * @param text The new raw name of the channel
       */
      void rawNameChanged( int channel, const QString& text );

      /// Signal emitted when the name of the time channel changes
      /**
       * @param channel The channel number
       * @param text The new name of the time channel
       */
      void timeNameChanged( int channel, const QString& text );
      /// Signal emitted when the number of the time histogram's channels changes
      /**
       * @param channel The channel number
       * @param channels The new number of time histogram channels
       */
      void timeChannelsChanged( int channel, int channels );
      /// Signal emitted when the time histogram's lower bound changes
      /**
       * @param channel The channel number
       * @param value The new time histogram lower bound
       */
      void timeLowerBoundChanged( int channel, double value );
      /// Signal emitted when the time histogram's upper bound changes
      /**
       * @param channel The channel number
       * @param value The new time histogram upper bound
       */
      void timeUpperBoundChanged( int channel, double value );

      /// Signal emitted when the name of the energy channel changes
      /**
       * @param channel The channel number
       * @param text The new name of the energy channel
       */
      void energyNameChanged( int channel, const QString& text );
      /// Signal emitted when the number of the energy histogram's channels changes
      /**
       * @param channel The channel number
       * @param channels The new number of energy histogram channels
       */
      void energyChannelsChanged( int channel, int channels );
      /// Signal emitted when the energy histogram's lower bound changes
      /**
       * @param channel The channel number
       * @param value The new energy histogram lower bound
       */
      void energyLowerBoundChanged( int channel, double value );
      /// Signal emitted when the energy histogram's upper bound changes
      /**
       * @param channel The channel number
       * @param value The new energy histogram upper bound
       */
      void energyUpperBoundChanged( int channel, double value );

   public slots:
      /// Slot for turning the channel on or off
      void setEnabled( bool on );
      /// Slot for setting the raw name of the channel
      void setRawName( const QString& text );

      /// Slot for setting the name of the time channel
      void setTimeName( const QString& text );
      /// Slot for setting the number of time histogram channels
      void setTimeChannels( int channels );
      /// Slot for setting the lower bound of the time histogram
      void setTimeLowerBound( double value );
      /// Slot for setting the upper bound of the time histogram
      void setTimeUpperBound( double value );

      /// Slot for setting the name of the energy channel
      void setEnergyName( const QString& text );
      /// Slot for setting the number of energy histogram channels
      void setEnergyChannels( int channels );
      /// Slot for setting the lower bound of the energy histogram
      void setEnergyLowerBound( double value );
      /// Slot for setting the upper bound of the energy histogram
      void setEnergyUpperBound( double value );

   private slots:
      /// Slot emitting enableChanged signals
      void enableChangedSlot( bool on );
      /// Slot emitting the rawNameChanged signals
      void rawNameChangedSlot( const QString& text );

      /// Slot emitting timeNameChanged signals
      void timeNameChangedSlot( const QString& text );
      /// Slot emitting timeChannelsChanged signals
      void timeChannelsChangedSlot( int channels );
      /// Slot emitting timeLowerBoundChanged signals
      void timeLowerBoundChangedSlot( double value );
      /// Slot emitting timeUpperBoundChanged signals
      void timeUpperBoundChangedSlot( double value );

      /// Slot emitting energyNameChanged signals
      void energyNameChangedSlot( const QString& text );
      /// Slot emitting energyChannelsChanged signals
      void energyChannelsChangedSlot( int channels );
      /// Slot emitting energyLowerBoundChanged signals
      void energyLowerBoundChangedSlot( double value );
      /// Slot emitting energyUpperBoundChanged signals
      void energyUpperBoundChangedSlot( double value );

   private:
      int m_channelNumber; ///< The channel described by this widget

      /// Widget turning the channel on or off
      UniquePtr< QCheckBox >::Type m_enabledEdit;
      /// Widget setting the channel's raw name
      UniquePtr< QLineEdit >::Type m_rawNameEdit;

      /// Description of the time channel
      UniquePtr< QLabel >::Type m_timeLabel;
      /// Widget setting the time channel's name
      UniquePtr< QLineEdit >::Type m_timeNameEdit;
      /// Widget setting the time histogram's channels
      UniquePtr< QSpinBox >::Type m_timeChannelsEdit;
      /// Widget setting the time histogram's lower bound
      UniquePtr< QDoubleSpinBox >::Type m_timeLowerBoundEdit;
      /// Widget setting the time histogram's upper bound
      UniquePtr< QDoubleSpinBox >::Type m_timeUpperBoundEdit;

      /// Description of the energy channel
      UniquePtr< QLabel >::Type m_energyLabel;
      /// Widget setting the energy channel's name
      UniquePtr< QLineEdit >::Type m_energyNameEdit;
      /// Widget setting the energy histogram's channels
      UniquePtr< QSpinBox >::Type m_energyChannelsEdit;
      /// Widget setting the energy histogram's lower bound
      UniquePtr< QDoubleSpinBox >::Type m_energyLowerBoundEdit;
      /// Widget setting the energy histogram's upper bound
      UniquePtr< QDoubleSpinBox >::Type m_energyUpperBoundEdit;

   }; // class ChannelGui

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_CHANNELGUI_H
