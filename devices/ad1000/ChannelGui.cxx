// $Id$

// Qt include(s):
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QtCore/QRect>

// Local include(s):
#include "ChannelGui.h"

namespace ad1000 {

const int ChannelGui::WIDTH = 320;
const int ChannelGui::HEIGHT = 25;

/**
 * The constructor sets up the look of the widget and connects up
 * all the signals and slots.
 *
 * @param parent The Qt parent of the widget
 * @param flags The Qt flags given to the widget
 */
ChannelGui::ChannelGui(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags) {

   //
   // Set the fixed size for the widget:
   //
   resize(WIDTH, HEIGHT);
   setMinimumSize(WIDTH, HEIGHT);
   setMaximumSize(WIDTH, HEIGHT);

   //
   // Create the widget changing the name of the channel:
   //
   m_nameEdit.reset(new QLineEdit(this));
   m_nameEdit->setGeometry(QRect(0, 2, 75, 21));
   connect(m_nameEdit.get(), SIGNAL(textChanged(const QString&)), this,
           SLOT(nameChangedSlot(const QString&)));

   //
   // Create the widget changing the number of channels in the
   // monitoring histogram:
   //
   m_channelsEdit.reset(new QSpinBox(this));
   m_channelsEdit->setGeometry(QRect(80, 0, 75, 25));
   m_channelsEdit->setRange(10, 8192);
   m_channelsEdit->setValue(8192);
   connect(m_channelsEdit.get(), SIGNAL(valueChanged(int)), this,
           SLOT(channelsChangedSlot(int)));

   //
   // Create the widget changing the lower bound of the monitoring
   // histogram:
   //
   m_lowerBoundEdit.reset(new QDoubleSpinBox(this));
   m_lowerBoundEdit->setGeometry(QRect(160, 0, 75, 25));
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
   m_upperBoundEdit->setGeometry(QRect(240, 0, 75, 25));
   m_upperBoundEdit->setDecimals(1);
   m_upperBoundEdit->setRange(-100000., 100000.);
   m_upperBoundEdit->setValue(8192.);
   connect(m_upperBoundEdit.get(), SIGNAL(valueChanged(double)), this,
           SLOT(upperBoundChangedSlot(double)));
}

QString ChannelGui::getName() const {

   return m_nameEdit->text();
}

int ChannelGui::getChannels() const {

   return m_channelsEdit->value();
}

double ChannelGui::getLowerBound() const {

   return m_lowerBoundEdit->value();
}

double ChannelGui::getUpperBound() const {

   return m_upperBoundEdit->value();
}

void ChannelGui::setName(const QString& text) {

   m_nameEdit->setText(text);
   return;
}

void ChannelGui::setChannels(int channels) {

   m_channelsEdit->setValue(channels);
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

void ChannelGui::nameChangedSlot(const QString& text) {

   emit nameChanged(text);
   return;
}

void ChannelGui::channelsChangedSlot(int channels) {

   emit channelsChanged(channels);
   return;
}

void ChannelGui::lowerBoundChangedSlot(double value) {

   emit lowerBoundChanged(value);
   return;
}

void ChannelGui::upperBoundChangedSlot(double value) {

   emit upperBoundChanged(value);
   return;
}

}  // namespace ad1000
