// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD2249A_CHANNELGUI_H
#define CDA_DEVICES_AD2249A_CHANNELGUI_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QWidget>

namespace ad2249a {

/**
 *  @short Class for modifying the properties of one input channel
 *
 *         Each input channel of the 2249A has the same properties
 *         that the user would want to change. These are the same
 *         properties that are stored in the ad2249a::ChannelConfig
 *         class.
 *
 *         This is a fixed size widget that has a Qt widget for changing
 *         each property of a channel. It looks like this on a Mac:
 *
 *         @image html t2228a_ChannelGui.png
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
   ChannelGui(int subaddress, QWidget* parent = 0,
              Qt::WindowFlags flags = Qt::WindowFlags());

   /// Get the subaddress described by this widget
   int getSubaddress() const;
   /// Set the subaddress described by this widget
   void setSubaddress(int value);

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
   void enableChanged(int subaddress, bool on);
   /// Signal emitted when the name of the channel changes
   /**
    * @param subaddress The subaddress of the channel
    * @param text The new name of the channel
    */
   void nameChanged(int subaddress, const QString& text);
   /// Signal emitted when the number of histogram channels changes
   /**
    * @param subaddress The subaddress of the channel
    * @param channels The new number of histogram channels
    */
   void channelsChanged(int subaddress, int channels);
   /// Signal emitted when the histogram lower bound changes
   /**
    * @param subaddress The subaddress of the channel
    * @param value The new histogram lower bound
    */
   void lowerBoundChanged(int subaddress, double value);
   /// Signal emitted when the histogram upper bound changes
   /**
    * @param subaddress The subaddress of the channel
    * @param value The new histogram upper bound
    */
   void upperBoundChanged(int subaddress, double value);

public slots:
   /// Slot for turning the channel on or off
   void setEnabled(bool on);
   /// Slot for setting the name of the channel
   void setName(const QString& text);
   /// Slot for setting the number of histogram channels
   void setChannels(int channels);
   /// Slot for setting the lower bound of the histogram
   void setLowerBound(double value);
   /// Slot for setting the upper bound of the histogram
   void setUpperBound(double value);

private slots:
   /// Slot emitting enableChanged signals
   void enableChangedSlot(bool on);
   /// Slot emitting nameChanged signals
   void nameChangedSlot(const QString& text);
   /// Slot emitting channelsChanged signals
   void channelsChangedSlot(int channels);
   /// Slot emitting lowerBoundChanged signals
   void lowerBoundChangedSlot(double value);
   /// Slot emitting upperBoundChanged signals
   void upperBoundChangedSlot(double value);

private:
   int m_subaddress;  ///< The subaddress described by this widget

   /// Widget turning the channel on or off
   std::unique_ptr<QCheckBox> m_enabledEdit;
   /// Widget setting the channel's name
   std::unique_ptr<QLineEdit> m_nameEdit;
   /// Widget setting the monitoring histogram's channels
   std::unique_ptr<QSpinBox> m_channelsEdit;
   /// Widget setting the monitoring histogram's lower bound
   std::unique_ptr<QDoubleSpinBox> m_lowerBoundEdit;
   /// Widget setting the monitoring histogram's upper bound
   std::unique_ptr<QDoubleSpinBox> m_upperBoundEdit;

};  // class ChannelGui

}  // namespace ad2249a

#endif  // CDA_DEVICES_AD2249A_CHANNELGUI_H
