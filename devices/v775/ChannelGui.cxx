// $Id$

// Qt include(s):
#include <QtCore/QRect>

// Local include(s):
#include "ChannelGui.h"

namespace v775 {

const int ChannelGui::WIDTH = 430;
const int ChannelGui::HEIGHT = 50;

/**
 * The constructor sets up the look of the widget and connects up
 * all the signals and slots.
 *
 * @param channel The channel number
 * @param parent The Qt parent of the widget
 * @param flags The Qt flags given to the widget
 */
ChannelGui::ChannelGui(int channel, QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), m_channel(channel) {

   //
   // Set the fixed size for the widget:
   //
   resize(WIDTH, HEIGHT);
   setMinimumSize(WIDTH, HEIGHT);
   setMaximumSize(WIDTH, HEIGHT);

   //
   // Create the widget enabling/disabling the channel:
   //
   m_enabledEdit.reset(
       new QCheckBox(tr("%1. channel").arg(m_channel + 1), this));
   m_enabledEdit->setGeometry(QRect(0, 0, 110, 25));
   connect(m_enabledEdit.get(), SIGNAL(toggled(bool)), this,
           SLOT(enableChangedSlot(bool)));

   //
   // Create the widget changing the name of the channel:
   //
   m_nameEdit.reset(new QLineEdit(this));
   m_nameEdit->setEnabled(false);
   m_nameEdit->setGeometry(QRect(115, 0, 75, 25));
   connect(m_nameEdit.get(), SIGNAL(textChanged(const QString&)), this,
           SLOT(nameChangedSlot(const QString&)));

   //
   // Create the widget changing the number of channels in the
   // monitoring histogram:
   //
   m_nbinsEdit.reset(new QSpinBox(this));
   m_nbinsEdit->setEnabled(false);
   m_nbinsEdit->setGeometry(QRect(195, 0, 75, 25));
   m_nbinsEdit->setRange(10, 4096);
   m_nbinsEdit->setValue(4096);
   connect(m_nbinsEdit.get(), SIGNAL(valueChanged(int)), this,
           SLOT(nbinsChangedSlot(int)));

   //
   // Create the widget changing the lower bound of the monitoring
   // histogram:
   //
   m_lowerBoundEdit.reset(new QDoubleSpinBox(this));
   m_lowerBoundEdit->setEnabled(false);
   m_lowerBoundEdit->setGeometry(QRect(275, 0, 75, 25));
   m_lowerBoundEdit->setDecimals(1);
   m_lowerBoundEdit->setRange(-100000., 100000.);
   m_lowerBoundEdit->setValue(0.);
   connect(m_lowerBoundEdit.get(), SIGNAL(valueChanged(double)), this,
           SLOT(lowerBoundChangedSlot(double)));

   //
   // Create the widget changing the upper bound of the monitoring
   // histogram:
   //
   m_upperBoundEdit.reset(new QDoubleSpinBox(this));
   m_upperBoundEdit->setEnabled(false);
   m_upperBoundEdit->setGeometry(QRect(355, 0, 75, 25));
   m_upperBoundEdit->setDecimals(1);
   m_upperBoundEdit->setRange(-100000., 100000.);
   m_upperBoundEdit->setValue(4096.);
   connect(m_upperBoundEdit.get(), SIGNAL(valueChanged(double)), this,
           SLOT(upperBoundChangedSlot(double)));

   //
   // Create a label for the threshold editor:
   //
   m_thresholdLabel.reset(new QLabel(tr("Readout threshold:"), this));
   m_thresholdLabel->setEnabled(false);
   m_thresholdLabel->setGeometry(QRect(40, 25, 155, 25));

   //
   // Create the widget for editing the channel's readout threshold:
   //
   m_thresholdEdit.reset(new QSpinBox(this));
   m_thresholdEdit->setEnabled(false);
   m_thresholdEdit->setGeometry(QRect(195, 25, 75, 25));
   m_thresholdEdit->setRange(0, 255);
   m_thresholdEdit->setValue(0);
   connect(m_thresholdEdit.get(), SIGNAL(valueChanged(int)), this,
           SLOT(thresholdChangedSlot(int)));
}

int ChannelGui::getChannel() const {

   return m_channel;
}

void ChannelGui::setChannel(int value) {

   m_channel = value;
   return;
}

/**
 * This function is normally called from the outside to change the
 * state of the channel. Normally when reading the device configuration
 * form file.
 */
void ChannelGui::setEnabled(bool on) {

   m_enabledEdit->setChecked(on);
   m_nameEdit->setEnabled(on);
   m_nbinsEdit->setEnabled(on);
   m_lowerBoundEdit->setEnabled(on);
   m_upperBoundEdit->setEnabled(on);
   m_thresholdLabel->setEnabled(on);
   m_thresholdEdit->setEnabled(on);
   return;
}

void ChannelGui::setName(const QString& text) {

   m_nameEdit->setText(text);
   return;
}

void ChannelGui::setNBins(int nbins) {

   m_nbinsEdit->setValue(nbins);
   return;
}

void ChannelGui::setLowerBound(double value) {

   m_lowerBoundEdit->setValue(value);
   return;
}

void ChannelGui::setUpperBound(double value) {

   m_upperBoundEdit->setValue(value);
   return;
}

void ChannelGui::setThreshold(int value) {

   m_thresholdEdit->setValue(value);
   return;
}

/**
 * This slot has to be a bit tricky. When you enable a channel,
 * you have to first enable the widgets changing the channel's
 * properties. Then, you have to make sure that the configuration
 * object has the same values as the ones shown, so the signals
 * from all the widgets have to be sent as if the values of the
 * widgets would've changed. But this can only be done when the
 * channel is turned on! (Otherwise you would try to change the
 * settings of a non-existent channel.)
 *
 * @param on State of the channel
 */
void ChannelGui::enableChangedSlot(bool on) {

   m_nameEdit->setEnabled(on);
   m_nbinsEdit->setEnabled(on);
   m_lowerBoundEdit->setEnabled(on);
   m_upperBoundEdit->setEnabled(on);
   m_thresholdLabel->setEnabled(on);
   m_thresholdEdit->setEnabled(on);

   emit enableChanged(m_channel, on);
   if (on) {
      emit nameChanged(m_channel, m_nameEdit->text());
      emit nbinsChanged(m_channel, m_nbinsEdit->value());
      emit lowerBoundChanged(m_channel, m_lowerBoundEdit->value());
      emit upperBoundChanged(m_channel, m_upperBoundEdit->value());
      emit thresholdChanged(m_channel, m_thresholdEdit->value());
   }

   return;
}

void ChannelGui::nameChangedSlot(const QString& text) {

   emit nameChanged(m_channel, text);
   return;
}

void ChannelGui::nbinsChangedSlot(int channels) {

   emit nbinsChanged(m_channel, channels);
   return;
}

void ChannelGui::lowerBoundChangedSlot(double value) {

   emit lowerBoundChanged(m_channel, value);
   return;
}

void ChannelGui::upperBoundChangedSlot(double value) {

   emit upperBoundChanged(m_channel, value);
   return;
}

void ChannelGui::thresholdChangedSlot(int value) {

   emit thresholdChanged(m_channel, value);
   return;
}

}  // namespace v775
