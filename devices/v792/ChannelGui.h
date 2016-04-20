// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V792_CHANNELGUI_H
#define CDA_DEVICES_V792_CHANNELGUI_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>

namespace v792 {

   /**
    *  @short Class for modifying the properties of one input channel
    *
    *         Each input channel of the V792 has the same properties
    *         that the user would want to change. These are the same
    *         properties that are stored in the v792::ChannelConfig
    *         class.
    *
    *         This is a fixed size widget that has a Qt widget for changing
    *         each property of a channel.
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

      /// Get the channel number described by this widget
      int getChannel() const;
      /// Set the channel described by this widget
      void setChannel( int value );

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
      /// Signal emitted when the name of the channel changes
      /**
       * @param channel The channel number
       * @param text The new name of the channel
       */
      void nameChanged( int channel, const QString& text );
      /// Signal emitted when the number of histogram bins changes
      /**
       * @param channel The channel number
       * @param bins The new number of histogram bins
       */
      void nbinsChanged( int channel, int bins );
      /// Signal emitted when the histogram lower bound changes
      /**
       * @param channel The channel number
       * @param value The new histogram lower bound
       */
      void lowerBoundChanged( int channel, double value );
      /// Signal emitted when the histogram upper bound changes
      /**
       * @param channel The channel number
       * @param value The new histogram upper bound
       */
      void upperBoundChanged( int channel, double value );
      /// Signal emitted when the readout threshold changes
      /**
       * @param channel The channel number
       * @param value The new threshold value
       */
      void thresholdChanged( int channel, int value );

   public slots:
      /// Slot for turning the channel on or off
      void setEnabled( bool on );
      /// Slot for setting the name of the channel
      void setName( const QString& text );
      /// Slot for setting the number of histogram bins
      void setNBins( int bins );
      /// Slot for setting the lower bound of the histogram
      void setLowerBound( double value );
      /// Slot for setting the upper bound of the histogram
      void setUpperBound( double value );
      /// Slot for setting the readout threshold of the channel
      void setThreshold( int value );

   private slots:
      /// Slot emitting enableChanged signals
      void enableChangedSlot( bool on );
      /// Slot emitting nameChanged signals
      void nameChangedSlot( const QString& text );
      /// Slot emitting nbinsChanged signals
      void nbinsChangedSlot( int channels );
      /// Slot emitting lowerBoundChanged signals
      void lowerBoundChangedSlot( double value );
      /// Slot emitting upperBoundChanged signals
      void upperBoundChangedSlot( double value );
      /// Slot emitting thresholdChanged signals
      void thresholdChangedSlot( int value );

   private:
      int m_channel; ///< The channel described by this widget

      /// Widget turning the channel on or off
      std::unique_ptr< QCheckBox > m_enabledEdit;
      /// Widget setting the channel's name
      std::unique_ptr< QLineEdit > m_nameEdit;
      /// Widget setting the monitoring histogram's number of bins
      std::unique_ptr< QSpinBox > m_nbinsEdit;
      /// Widget setting the monitoring histogram's lower bound
      std::unique_ptr< QDoubleSpinBox > m_lowerBoundEdit;
      /// Widget setting the monitoring histogram's upper bound
      std::unique_ptr< QDoubleSpinBox > m_upperBoundEdit;
      /// Label for the threshold editor
      std::unique_ptr< QLabel > m_thresholdLabel;
      /// Widget setting the readout threshold value
      std::unique_ptr< QSpinBox > m_thresholdEdit;

   }; // class ChannelGui

} // namespace v792

#endif // CDA_DEVICES_V792_CHANNELGUI_H
