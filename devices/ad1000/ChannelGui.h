// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_CHANNELGUI_H
#define CDA_DEVICES_AD1000_CHANNELGUI_H

// Qt include(s):
#include <QtGui/QWidget>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QSpinBox )
QT_FORWARD_DECLARE_CLASS( QDoubleSpinBox )

namespace ad1000 {

   // Bring the Qt classes into this namespace:
   using QT_PREPEND_NAMESPACE( QLineEdit );
   using QT_PREPEND_NAMESPACE( QSpinBox );
   using QT_PREPEND_NAMESPACE( QDoubleSpinBox );

   /**
    *  @short Class for modifying the properties of the input channel
    *
    *         The input channel of the AD1000 has some properties
    *         that the user may want to change. These are the same
    *         properties that are stored in the ad1000::ChannelConfig
    *         class.
    *
    *         This is a fixed size widget that has a Qt widget for changing
    *         each property of the channel.
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
      ChannelGui( QWidget* parent = 0,
                  Qt::WindowFlags flags = 0 );
      /// Destructor
      ~ChannelGui();

      /// Total width of the widget
      static const int WIDTH;
      /// Total height of the widget
      static const int HEIGHT;

      /// Get the name of the channel
      QString getName() const;
      /// Get the channel number of the monitoring histogram
      int getChannels() const;
      /// Get the lower bound of the monitoring histogram
      double getLowerBound() const;
      /// Get the upper bound of the monitoring histogram
      double getUpperBound() const;

   signals:
      /// Signal emitted when the name of the channel changes
      /**
       * @param text The new name of the channel
       */
      void nameChanged( const QString& text );
      /// Signal emitted when the number of histogram channels changes
      /**
       * @param channels The new number of histogram channels
       */
      void channelsChanged( int channels );
      /// Signal emitted when the histogram lower bound changes
      /**
       * @param value The new histogram lower bound
       */
      void lowerBoundChanged( double value );
      /// Signal emitted when the histogram upper bound changes
      /**
       * @param value The new histogram upper bound
       */
      void upperBoundChanged( double value );

   public slots:
      /// Slot for setting the name of the channel
      void setName( const QString& text );
      /// Slot for setting the number of histogram channels
      void setChannels( int channels );
      /// Slot for setting the lower bound of the histogram
      void setLowerBound( double value );
      /// Slot for setting the upper bound of the histogram
      void setUpperBound( double value );

   private slots:
      /// Slot emitting nameChanged signals
      void nameChangedSlot( const QString& text );
      /// Slot emitting channelsChanged signals
      void channelsChangedSlot( int channels );
      /// Slot emitting lowerBoundChanged signals
      void lowerBoundChangedSlot( double value );
      /// Slot emitting upperBoundChanged signals
      void upperBoundChangedSlot( double value );

   private:
      QLineEdit*      m_nameEdit; ///< Widget setting the channel's name
      QSpinBox*       m_channelsEdit; ///< Widget setting the histogram's channels
      QDoubleSpinBox* m_lowerBoundEdit; ///< Widget setting the histogram's lower bound
      QDoubleSpinBox* m_upperBoundEdit; ///< Widget setting the histogram's upper bound

   }; // class ChannelGui

} // namespace ad1000

#endif // CDA_DEVICES_AD1000_CHANNELGUI_H
