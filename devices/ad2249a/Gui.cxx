// $Id$

// Qt include(s):
#include <QCheckBox>
#include <QFont>
#include <QLabel>
#include <QPainter>
#include <QScrollArea>
#include <QtCore/QLine>
#include <QtCore/QRect>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "Gui.h"

namespace ad2249a {

/**
 * The constructor creates all member objects and arranges them
 * in the area given to the device.
 *
 * @param parent The Qt parent of the widget
 * @param flags The Qt flags given to the widget
 */
Gui::Gui(QWidget* parent, Qt::WindowFlags flags)
    : dev::CamacGui(parent, flags), m_logger("ad2249a::Gui") {

   //
   // Create the widget that will hold all the configuration widgets:
   //
   m_scrollWidget.reset(new QWidget(0, flags));
   m_scrollWidget->setGeometry(QRect(0, 0, WIDGET_WIDTH - 20, 450));

   //
   // Embed the previous widget into a scroll area:
   //
   m_scrollArea.reset(new QScrollArea(this));
   m_scrollArea->setGeometry(QRect(0, 20, WIDGET_WIDTH, WIDGET_HEIGHT - 20));
   m_scrollArea->setWidget(m_scrollWidget.get());

   //
   // Create a label telling us what kind of device this is:
   //
   m_topLabel.reset(
       new QLabel(tr("LeCroy 2249A 12 channel Analog-to-Digital Converter"),
                  m_scrollWidget.get()));
   m_topLabel->setAlignment(Qt::AlignCenter);
   m_topLabel->setGeometry(QRect(25, 30, 430, 25));

   //
   // Create a label identifying the channel name settings:
   //
   m_nameLabel.reset(new QLabel(tr("Name"), m_scrollWidget.get()));
   m_nameLabel->setAlignment(Qt::AlignCenter);
   m_nameLabel->setGeometry(QRect(140, 75, 75, 25));

   //
   // Create a label identifying the histogram channel settings:
   //
   m_channelsLabel.reset(new QLabel(tr("Channels"), m_scrollWidget.get()));
   m_channelsLabel->setAlignment(Qt::AlignCenter);
   m_channelsLabel->setGeometry(QRect(220, 75, 75, 25));

   //
   // Create a label identifying the histogram lower bound settings:
   //
   m_lowerBoundLabel.reset(new QLabel(tr("Lower"), m_scrollWidget.get()));
   m_lowerBoundLabel->setAlignment(Qt::AlignCenter);
   m_lowerBoundLabel->setGeometry(QRect(300, 75, 75, 25));

   //
   // Create a label identifying the histogram upper bound settings:
   //
   m_upperBoundLabel.reset(new QLabel(tr("Upper"), m_scrollWidget.get()));
   m_upperBoundLabel->setAlignment(Qt::AlignCenter);
   m_upperBoundLabel->setGeometry(QRect(380, 75, 75, 25));

   //
   // Create the widgets modifying the settings of the input channels:
   //
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {

      m_gchannels[i].reset(new ChannelGui(i, m_scrollWidget.get()));
      m_gchannels[i]->setGeometry(
          QRect(25, 100 + i * 25, ChannelGui::WIDTH, ChannelGui::HEIGHT));
      connect(m_gchannels[i].get(), SIGNAL(enableChanged(int, bool)), this,
              SLOT(channelEnabledSlot(int, bool)));
      connect(m_gchannels[i].get(), SIGNAL(nameChanged(int, const QString&)),
              this, SLOT(nameChangedSlot(int, const QString&)));
      connect(m_gchannels[i].get(), SIGNAL(channelsChanged(int, int)), this,
              SLOT(channelsChangedSlot(int, int)));
      connect(m_gchannels[i].get(), SIGNAL(lowerBoundChanged(int, double)),
              this, SLOT(lowerBoundChangedSlot(int, double)));
      connect(m_gchannels[i].get(), SIGNAL(upperBoundChanged(int, double)),
              this, SLOT(upperBoundChangedSlot(int, double)));
   }

   //
   // Create the widget modifying the LAM generation setting:
   //
   m_generateLamEdit.reset(
       new QCheckBox(tr("Generate LAM"), m_scrollWidget.get()));
   m_generateLamEdit->setGeometry(QRect(25, 410, 120, 25));
   connect(m_generateLamEdit.get(), SIGNAL(toggled(bool)), this,
           SLOT(generateLamChangedSlot(bool)));
}

/**
 * The function calls the ad2249a::Device::readConfig(QIODevice*) function
 * to read the device configuration and then calls sync() to show the
 * new configuration correctly.
 */
StatusCode Gui::readConfig(QIODevice& dev) {

   CHECK(Device::readConfig(dev));
   sync();
   return StatusCode::SUCCESS;
}

/**
 * The function calls the ad2249a::Device::readConfig(QDomElement&) function
 * to read the device configuration and then calls sync() to show the
 * new configuration correctly.
 */
StatusCode Gui::readConfig(const QDomElement& node) {

   CHECK(Device::readConfig(node));
   sync();
   return StatusCode::SUCCESS;
}

/**
 * The function draws a simple representation of the CAMAC module
 * in a 20x200 pixel format. The input channels are shown in green
 * when enabled, and red when disabled. Additionally a small "LAM"
 * label is shown at the bottom if LAM generation is enabled.
 */
void Gui::drawModule(QPainter& painter) const {

   //
   // Draw the background color:
   //
   painter.setPen(Qt::blue);
   painter.setBrush(Qt::blue);
   painter.drawRect(QRect(0, 0, 20, 200));

   //
   // Draw a "ADC" sign on the top:
   //
   QFont font = painter.font();
#ifdef Q_OS_DARWIN
   font.setPointSize(8);
#else
   font.setPointSize(6);
#endif
   painter.setFont(font);
   painter.setPen(Qt::white);
   painter.setBrush(Qt::white);
   painter.drawText(QRect(0, 0, 20, 15), Qt::AlignCenter, tr("ADC"));
   painter.drawLine(QLine(3, 15, 17, 15));

   //
   // Draw the common-stop and common-start NIM inputs:
   //
   painter.setPen(Qt::black);
   painter.setBrush(Qt::gray);
   painter.drawEllipse(QRect(7, 35, 6, 6));
   painter.drawEllipse(QRect(7, 45, 6, 6));
   painter.setPen(Qt::white);
   painter.setBrush(Qt::white);
   painter.drawLine(QLine(3, 43, 17, 43));
   painter.drawLine(QLine(3, 53, 17, 53));

   //
   // Draw the stop NIM inputs for the individual channels:
   //
   painter.setPen(Qt::black);
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      if (m_channels[i]) {
         painter.setBrush(Qt::green);
      } else {
         painter.setBrush(Qt::red);
      }
      painter.drawEllipse(QRect(7, 55 + 9 * i, 6, 6));
   }

   //
   // Draw the clear NIM input:
   //
   painter.setPen(Qt::white);
   painter.setBrush(Qt::white);
   painter.drawLine(QLine(3, 168, 17, 168));
   painter.setPen(Qt::black);
   painter.setBrush(Qt::gray);
   painter.drawEllipse(QRect(7, 170, 6, 6));

   //
   // Draw a "LAM" sign on the bottom if the module is set to
   // generate LAM:
   //
   if (m_generateLam) {
      painter.setPen(Qt::green);
      painter.setBrush(Qt::green);
      painter.drawText(QRect(0, 185, 20, 15), Qt::AlignCenter, tr("LAM"));
   }

   return;
}

int Gui::deviceWidth() const {

   return 1;
}

void Gui::channelEnabledSlot(int subaddress, bool on) {

   if (on) {
      m_channels[subaddress].reset(new ChannelConfig());
      m_channels[subaddress]->setSubaddress(subaddress);
   } else {
      m_channels[subaddress].reset();
   }

   emit redrawModule();
   return;
}

void Gui::nameChangedSlot(int subaddress, const QString& text) {

   m_channels[subaddress]->setName(text);
   return;
}

void Gui::channelsChangedSlot(int subaddress, int channels) {

   m_channels[subaddress]->setNumberOfChannels(channels);
   return;
}

void Gui::lowerBoundChangedSlot(int subaddress, double value) {

   m_channels[subaddress]->setLowerBound(value);
   return;
}

void Gui::upperBoundChangedSlot(int subaddress, double value) {

   m_channels[subaddress]->setUpperBound(value);
   return;
}

void Gui::generateLamChangedSlot(bool on) {

   m_generateLam = on;
   emit redrawModule();
   return;
}

/**
 * After a configuration is read from file, the graphical objects have
 * to be sync-ed to show this new configuration. This function takes
 * care of doing that.
 */
void Gui::sync() {

   //
   // For each subaddress the adjustments to the graphical objects
   // have to be made in a tricky way. The changes have to be made
   // while the graphical channel is disabled. Otherwise the signals
   // sent from the channel as its values are changing would create
   // "strange" effects.
   //
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      if (m_channels[i]) {
         m_gchannels[i]->setEnabled(false);
         m_gchannels[i]->setName(m_channels[i]->getName());
         m_gchannels[i]->setChannels(m_channels[i]->getNumberOfChannels());
         m_gchannels[i]->setLowerBound(m_channels[i]->getLowerBound());
         m_gchannels[i]->setUpperBound(m_channels[i]->getUpperBound());
         m_gchannels[i]->setEnabled(true);
      } else {
         m_gchannels[i]->setEnabled(false);
      }
   }

   m_generateLamEdit->setChecked(m_generateLam);

   return;
}

}  // namespace ad2249a
