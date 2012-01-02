// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_CHANNELGUI_H
#define CDA_DEVICES_AD413A_CHANNELGUI_H

// Qt include(s):
#include <QtGui/QWidget>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QSpinBox )
QT_FORWARD_DECLARE_CLASS( QDoubleSpinBox )

namespace ad413a {

   /**
    *  @short Class for modifying the properties of one input channel
    *
    *         Each input channel of the AD413A has the same properties
    *         that the user would want to change. These are the same
    *         properties that are stored in the ad413a::ChannelConfig
    *         class.
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
      ChannelGui( int subaddress, QWidget* parent = 0,
                  Qt::WindowFlags flags = 0 );
      /// Destructor
      ~ChannelGui();

      /// Get the subaddress described by this widget
      int getSubaddress() const;
      /// Set the subaddress described by this widget
      void setSubaddress( int value );

      /// Total width of the widget
      static const int WIDTH;
      /// Total height of the widget
      static const int HEIGHT;

   signals:
      /// Signal emitted when the channel is turned on or off
      /**
       * @param subaddress The subaddress of the channel
       * @param on The state of the object
       */
      void enableChanged( int subaddress, bool on );
      /// Signal emitted when the name of the channel changes
      /**
       * @param subaddress The subaddress of the channel
       * @param text The new name of the channel
       */
      void nameChanged( int subaddress, const QString& text );
      /// Signal emitted when the LLD of the channel changes
      /**
       * @param subaddress The subaddress of the channel
       * @param lld The new low level discriminant of the channel
       */
      void lldChanged( int subaddess, int lld );
      /// Signal emitted when the number of histogram channels changes
      /**
       * @param subaddress The subaddress of the channel
       * @param channels The new number of histogram channels
       */
      void channelsChanged( int subaddress, int channels );
      /// Signal emitted when the histogram lower bound changes
      /**
       * @param subaddress The subaddress of the channel
       * @param value The new histogram lower bound
       */
      void lowerBoundChanged( int subaddress, double value );
      /// Signal emitted when the histogram upper bound changes
      /**
       * @param subaddress The subaddress of the channel
       * @param value The new histogram upper bound
       */
      void upperBoundChanged( int subaddress, double value );

   public slots:
      /// Slot for turning the channel on or off
      void setEnabled( bool on );
      /// Slot for setting the name of the channel
      void setName( const QString& text );
      /// Slot for setting the low level discriminant of the channel
      void setLowLevelDiscriminant( int lld );
      /// Slot for setting the number of histogram channels
      void setChannels( int channels );
      /// Slot for setting the lower bound of the histogram
      void setLowerBound( double value );
      /// Slot for setting the upper bound of the histogram
      void setUpperBound( double value );

   private slots:
      /// Slot emitting enableChanged signals
      void enableChangedSlot( bool on );
      /// Slot emitting nameChanged signals
      void nameChangedSlot( const QString& text );
      /// Slot emitting lldChanged signals
      void lldChangedSlot( int lld );
      /// Slot emitting channelsChanged signals
      void channelsChangedSlot( int channels );
      /// Slot emitting lowerBoundChanged signals
      void lowerBoundChangedSlot( double value );
      /// Slot emitting upperBoundChanged signals
      void upperBoundChangedSlot( double value );

   private:
      int             m_subaddress; ///< The subaddress described by this widget

      QCheckBox*      m_enabledEdit; ///< Widget turning the channel on or off
      QLineEdit*      m_nameEdit; ///< Widget setting the channel's name
      QSpinBox*       m_lldEdit; ///< Widget setting the low level discriminant of the channel
      QSpinBox*       m_channelsEdit; ///< Widget setting the histogram's channels
      QDoubleSpinBox* m_lowerBoundEdit; ///< Widget setting the histogram's lower bound
      QDoubleSpinBox* m_upperBoundEdit; ///< Widget setting the histogram's upper bound

   }; // class ChannelGui

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_CHANNELGUI_H
