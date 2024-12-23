// $Id$

// Qt include(s):
#include <QCheckBox>
#include <QComboBox>
#include <QFont>
#include <QGroupBox>
#include <QLabel>
#include <QPixmap>
#include <QScrollArea>
#include <QSpinBox>
#include <QtCore/QLine>
#include <QtCore/QRect>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "ChannelGui.h"
#include "GroupGui.h"
#include "Gui.h"

namespace dt5740 {

/**
 * The constructor creates all member objects and arranges them
 * in the area given to the device.
 *
 * @param parent The Qt parent of the widget
 * @param flags The Qt flags given to the widget
 */
Gui::Gui(QWidget* parent, Qt::WindowFlags flags)
    : dev::CaenDigitizerGui(parent, flags), m_logger("dt5740::Gui") {

   //
   // Create the widget that will hold all the configuration widgets:
   //
   m_scrollWidget.reset(new QWidget(0, flags));
   m_scrollWidget->setGeometry(QRect(0, 0, WIDGET_WIDTH - 20, 4440));

   //
   // Embed the previous widget into a scroll area:
   //
   m_scrollArea.reset(new QScrollArea(this));
   m_scrollArea->setGeometry(QRect(0, 0, WIDGET_WIDTH, WIDGET_HEIGHT));
   m_scrollArea->setWidget(m_scrollWidget.get());

   //
   // Put a picture of the digitizer at the top:
   //
   m_image.reset(new QLabel(m_scrollWidget.get()));
   m_image->setGeometry(QRect(50, 10, 400, 181));
   QPixmap pixmap(":/img/dt5740.png");
#if QT_VERSION >= QT_VERSION_CHECK(5, 1, 0)
   pixmap.setDevicePixelRatio(m_image->devicePixelRatio());
#endif  // QT_VERSION
   m_image->setPixmap(pixmap);
   m_image->setScaledContents(true);

   //
   // Create a label telling us what kind of device this is:
   //
   m_topLabel.reset(new QLabel(tr("CAEN DT5740 32 Channel 12 bit "
                                  "62.5MS/s Digitizer"),
                               m_scrollWidget.get()));
   m_topLabel->setAlignment(Qt::AlignCenter);
   m_topLabel->setGeometry(QRect(20, 200, WIDGET_WIDTH - 40, 35));
   QFont titleFont(m_topLabel->font());
   titleFont.setPointSize(titleFont.pointSize() + 2);
   titleFont.setBold(true);
   m_topLabel->setFont(titleFont);

   //
   // Create the device connection settings:
   //
   m_connectionBox.reset(
       new QGroupBox(tr("Connection parameters"), m_scrollWidget.get()));
   m_connectionBox->setGeometry(QRect(10, 245, WIDGET_WIDTH - 40, 90));

   m_connModeLabel.reset(
       new QLabel(tr("Connection mode:"), m_connectionBox.get()));
   m_connModeLabel->setGeometry(QRect(10, 25, 130, 25));

   m_connMode.reset(new QComboBox(m_connectionBox.get()));
   m_connMode->setGeometry(QRect(180, 25, 250, 25));
   m_connMode->addItem(tr("USB"));
   m_connMode->addItem(tr("PCI Optical Link"));
   m_connMode->addItem(tr("PCIe Optical Link"));
   m_connMode->addItem(tr("PCIe Embedded Digitizer"));
   connect(m_connMode.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(connectionModeSlot(int)));

   m_connLinkLabel.reset(new QLabel(tr("Link number:"), m_connectionBox.get()));
   m_connLinkLabel->setGeometry(QRect(10, 55, 130, 25));

   m_connLink.reset(new QSpinBox(m_connectionBox.get()));
   m_connLink->setGeometry(QRect(180, 55, 150, 25));
   m_connLink->setRange(0, 10);
   m_connLink->setToolTip(
       tr("In USB mode this means the ID given by "
          "the OS to the USB connection. If this is "
          "the only such device connected by USB, "
          "it will be \"0\"."));
   connect(m_connLink.get(), SIGNAL(valueChanged(int)), this,
           SLOT(connectionLinkSlot(int)));

   //
   // Create the trigger settings:
   //
   m_triggerBox.reset(
       new QGroupBox(tr("Trigger settings"), m_scrollWidget.get()));
   m_triggerBox->setGeometry(QRect(10, 345, WIDGET_WIDTH - 40, 240));

   m_trigOvlpEnabledWidget.reset(new QCheckBox(tr("Enable trigger "
                                                  "overlaps"),
                                               m_triggerBox.get()));
   m_trigOvlpEnabledWidget->setGeometry(QRect(10, 25, 400, 25));
   connect(m_trigOvlpEnabledWidget.get(), SIGNAL(toggled(bool)), this,
           SLOT(trigOvlpEnabledSlot(bool)));

   m_extTrigEnabledWidget.reset(new QCheckBox(tr("Enable external "
                                                 "trigger"),
                                              m_triggerBox.get()));
   m_extTrigEnabledWidget->setGeometry(QRect(10, 55, 400, 25));
   connect(m_extTrigEnabledWidget.get(), SIGNAL(toggled(bool)), this,
           SLOT(extTrigEnabledSlot(bool)));

   m_extTrigOutEnabledWidget.reset(new QCheckBox(tr("Forward external "
                                                    "trigger to front "
                                                    "panel"),
                                                 m_triggerBox.get()));
   m_extTrigOutEnabledWidget->setGeometry(QRect(10, 85, 400, 25));
   connect(m_extTrigOutEnabledWidget.get(), SIGNAL(toggled(bool)), this,
           SLOT(extTrigOutEnabledSlot(bool)));

   m_highImpedanceGPOWidget.reset(new QCheckBox(tr("Assert high "
                                                   "impedance on GPO "
                                                   "output"),
                                                m_triggerBox.get()));
   m_highImpedanceGPOWidget->setGeometry(QRect(10, 115, 400, 25));
   connect(m_highImpedanceGPOWidget.get(), SIGNAL(toggled(bool)), this,
           SLOT(highImpedanceGPOSlot(bool)));

   m_postTrigPercentageLabel.reset(new QLabel(tr("Post trigger "
                                                 "percentage:"),
                                              m_triggerBox.get()));
   m_postTrigPercentageLabel->setGeometry(QRect(10, 145, 150, 25));

   m_postTrigPercentageWidget.reset(new QSpinBox(m_triggerBox.get()));
   m_postTrigPercentageWidget->setGeometry(QRect(180, 145, 150, 25));
   m_postTrigPercentageWidget->setRange(0, 100);
   m_postTrigPercentageWidget->setValue(100);
   m_postTrigPercentageWidget->setToolTip(
       tr("You can set here how many "
          "of the collected samples "
          "should be taken after the "
          "trigger signal."));
   connect(m_postTrigPercentageWidget.get(), SIGNAL(valueChanged(int)), this,
           SLOT(postTrigPercentageSlot(int)));

   m_trigModeLabel.reset(new QLabel(tr("Trigger mode:"), m_triggerBox.get()));
   m_trigModeLabel->setGeometry(QRect(10, 175, 150, 25));

   m_trigModeWidget.reset(new QComboBox(m_triggerBox.get()));
   m_trigModeWidget->setGeometry(QRect(180, 175, 250, 25));
   m_trigModeWidget->addItem(tr("Trigger on input over threshold"));
   m_trigModeWidget->addItem(tr("Trigger on input under threshold"));
   connect(m_trigModeWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(trigModeSlot(int)));

   m_signalTypeLabel.reset(
       new QLabel(tr("Front panel signal type:"), m_triggerBox.get()));
   m_signalTypeLabel->setGeometry(QRect(10, 205, 150, 25));

   m_signalTypeWidget.reset(new QComboBox(m_triggerBox.get()));
   m_signalTypeWidget->setGeometry(QRect(180, 205, 250, 25));
   m_signalTypeWidget->addItem(tr("NIM"));
   m_signalTypeWidget->addItem(tr("TTL"));
   connect(m_signalTypeWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(signalTypeSlot(int)));

   //
   // Create the data acquisition settings:
   //
   m_acquisitionBox.reset(
       new QGroupBox(tr("Data acquisition settings"), m_scrollWidget.get()));
   m_acquisitionBox->setGeometry(QRect(10, 595, WIDGET_WIDTH - 40, 240));

   m_patGenEnabledWidget.reset(new QCheckBox(tr("Enable test pattern "
                                                "generation"),
                                             m_acquisitionBox.get()));
   m_patGenEnabledWidget->setGeometry(QRect(10, 25, 400, 25));
   connect(m_patGenEnabledWidget.get(), SIGNAL(toggled(bool)), this,
           SLOT(patGenEnabledSlot(bool)));

   m_saveRawNtupleWidget.reset(new QCheckBox(tr("Save raw data to "
                                                "ntuple"),
                                             m_acquisitionBox.get()));
   m_saveRawNtupleWidget->setGeometry(QRect(10, 55, 400, 25));
   connect(m_saveRawNtupleWidget.get(), SIGNAL(toggled(bool)), this,
           SLOT(saveRawNtupleSlot(bool)));

   m_acqModeLabel.reset(
       new QLabel(tr("Acquisition mode:"), m_acquisitionBox.get()));
   m_acqModeLabel->setGeometry(QRect(10, 85, 150, 25));

   m_acqModeWidget.reset(new QComboBox(m_acquisitionBox.get()));
   m_acqModeWidget->setGeometry(QRect(180, 85, 250, 25));
   m_acqModeWidget->addItem(tr("Register controlled"));
   m_acqModeWidget->addItem(tr("GPI controlled"));
   connect(m_acqModeWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(acqModeSlot(int)));

   m_gateModeLabel.reset(new QLabel(tr("Gate mode:"), m_acquisitionBox.get()));
   m_gateModeLabel->setGeometry(QRect(10, 115, 150, 25));

   m_gateModeWidget.reset(new QComboBox(m_acquisitionBox.get()));
   m_gateModeWidget->setGeometry(QRect(180, 115, 250, 25));
   m_gateModeWidget->addItem(tr("Window"));
   m_gateModeWidget->addItem(tr("Single shot"));
   connect(m_gateModeWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(gateModeSlot(int)));

   m_bufferModeLabel.reset(
       new QLabel(tr("Buffer mode:"), m_acquisitionBox.get()));
   m_bufferModeLabel->setGeometry(QRect(10, 145, 150, 25));

   m_bufferModeWidget.reset(new QComboBox(m_acquisitionBox.get()));
   m_bufferModeWidget->setGeometry(QRect(180, 145, 250, 25));
   m_bufferModeWidget->setToolTip(
       tr("You can choose how many samples "
          "should be collected after each "
          "trigger, using this property."));
   m_bufferModeWidget->addItem(tr("1 buffer with 192k samples"));
   m_bufferModeWidget->addItem(tr("2 buffers with 96k samples"));
   m_bufferModeWidget->addItem(tr("4 buffers with 48k samples"));
   m_bufferModeWidget->addItem(tr("8 buffers with 24k samples"));
   m_bufferModeWidget->addItem(tr("16 buffers with 12k samples"));
   m_bufferModeWidget->addItem(tr("32 buffers with 6k samples"));
   m_bufferModeWidget->addItem(tr("64 buffers with 3k samples"));
   m_bufferModeWidget->addItem(tr("128 buffers with 1536 samples"));
   m_bufferModeWidget->addItem(tr("256 buffers with 768 samples"));
   m_bufferModeWidget->addItem(tr("512 buffers with 384 samples"));
   m_bufferModeWidget->addItem(tr("1024 buffers with 192 samples"));
   connect(m_bufferModeWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(bufferModeSlot(int)));

   m_clockSourceLabel.reset(
       new QLabel(tr("Clock source:"), m_acquisitionBox.get()));
   m_clockSourceLabel->setGeometry(QRect(10, 175, 150, 25));

   m_clockSourceWidget.reset(new QComboBox(m_acquisitionBox.get()));
   m_clockSourceWidget->setGeometry(QRect(180, 175, 250, 25));
   m_clockSourceWidget->setToolTip(
       tr("You can choose where the device "
          "should get its clock for the "
          "digitization timing."));
   m_clockSourceWidget->addItem(tr("Internal"));
   m_clockSourceWidget->addItem(tr("External"));
   connect(m_clockSourceWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(clockSourceSlot(int)));

   m_evCountModeLabel.reset(
       new QLabel(tr("Event count mode:"), m_acquisitionBox.get()));
   m_evCountModeLabel->setGeometry(QRect(10, 205, 150, 25));

   m_evCountModeWidget.reset(new QComboBox(m_acquisitionBox.get()));
   m_evCountModeWidget->setGeometry(QRect(180, 205, 250, 25));
   m_evCountModeWidget->setToolTip(
       tr("You can choose how the number of "
          "events should be counted by the "
          "device."));
   m_evCountModeWidget->addItem(tr("Count accepted triggers"));
   m_evCountModeWidget->addItem(tr("Count all triggers"));
   connect(m_evCountModeWidget.get(), SIGNAL(currentIndexChanged(int)), this,
           SLOT(evCountModeSlot(int)));

   //
   // Create the signal reconstruction settings:
   //
   m_recoBox.reset(new QGroupBox(tr("Signal reconstruction settings"),
                                 m_scrollWidget.get()));
   m_recoBox->setGeometry(QRect(10, 845, WIDGET_WIDTH - 40, 150));

   m_cfdFractionLabel.reset(new QLabel(tr("CFD fraction:"), m_recoBox.get()));
   m_cfdFractionLabel->setGeometry(QRect(10, 25, 150, 25));

   m_cfdFractionWidget.reset(new QDoubleSpinBox(m_recoBox.get()));
   m_cfdFractionWidget->setGeometry(QRect(180, 25, 250, 25));
   m_cfdFractionWidget->setRange(-100.0, 100.0);
   connect(m_cfdFractionWidget.get(), SIGNAL(valueChanged(double)), this,
           SLOT(cfdFractionSlot(double)));

   m_cfdDelayLabel.reset(new QLabel(tr("CFD delay:"), m_recoBox.get()));
   m_cfdDelayLabel->setGeometry(QRect(10, 55, 150, 25));

   m_cfdDelayWidget.reset(new QSpinBox(m_recoBox.get()));
   m_cfdDelayWidget->setGeometry(QRect(180, 55, 250, 25));
   m_cfdDelayWidget->setRange(0, 1000);
   connect(m_cfdDelayWidget.get(), SIGNAL(valueChanged(int)), this,
           SLOT(cfdDelaySlot(int)));

   m_cfdLengthLabel.reset(new QLabel(tr("CFD length:"), m_recoBox.get()));
   m_cfdLengthLabel->setGeometry(QRect(10, 85, 150, 25));

   m_cfdLengthWidget.reset(new QSpinBox(m_recoBox.get()));
   m_cfdLengthWidget->setGeometry(QRect(180, 85, 250, 25));
   m_cfdLengthWidget->setRange(2, 1000);
   connect(m_cfdLengthWidget.get(), SIGNAL(valueChanged(int)), this,
           SLOT(cfdLengthSlot(int)));

   m_gaussSmoothWidthLabel.reset(
       new QLabel(tr("Gauss. smooth width:"), m_recoBox.get()));
   m_gaussSmoothWidthLabel->setGeometry(QRect(10, 115, 150, 25));

   m_gaussSmoothWidthWidget.reset(new QDoubleSpinBox(m_recoBox.get()));
   m_gaussSmoothWidthWidget->setGeometry(QRect(180, 115, 250, 25));
   m_gaussSmoothWidthWidget->setRange(0.0, 10.0);
   connect(m_gaussSmoothWidthWidget.get(), SIGNAL(valueChanged(double)), this,
           SLOT(gaussSmoothWidthSlot(double)));

   //
   // Create the channel groups:
   //
   for (int i = 0; i < NUMBER_OF_GROUPS; ++i) {

      // Create a new channel group:
      m_ggroups[i].reset(new GroupGui(m_groups[i], m_scrollWidget.get()));
      m_ggroups[i]->setGeometry(QRect(5, 1005 + i * (GroupGui::HEIGHT + 10),
                                      GroupGui::WIDTH, GroupGui::HEIGHT));
   }

   // Take the default values set in the base class:
   sync();
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

void Gui::connectionModeSlot(int index) {

   switch (index) {

      case 0:
         m_connType = caen::Digitizer::USB;
         break;
      case 1:
         m_connType = caen::Digitizer::PCI_OpticalLink;
         break;
      case 2:
         m_connType = caen::Digitizer::PCIE_OpticalLink;
         break;
      case 3:
         m_connType = caen::Digitizer::PCIE_EmbeddedDigitizer;
         break;
      default:
         REPORT_ERROR(tr("Connection mode not recognized"));
         m_connType = caen::Digitizer::USB;
         break;
   }

   // Tell the crate that the device's ID changed:
   emit idChanged();

   return;
}

void Gui::connectionLinkSlot(int link) {

   // Remember the link number:
   m_linkNumber = link;

   // Tell the crate that the device's ID changed:
   emit idChanged();

   return;
}

void Gui::trigOvlpEnabledSlot(bool checked) {

   m_trigOvlpEnabled = checked;
   return;
}

void Gui::extTrigEnabledSlot(bool checked) {

   m_extTrigEnabled = checked;
   return;
}

void Gui::extTrigOutEnabledSlot(bool checked) {

   m_extTrigOutEnabled = checked;
   return;
}

void Gui::trigModeSlot(int index) {

   // Translate the state of the combo box into an enumeration value:
   switch (index) {

      case 0:
         m_trigMode = TRG_InputOverThreshold;
         break;
      case 1:
         m_trigMode = TRG_InputUnderThreshold;
         break;
      default:
         REPORT_ERROR(tr("Trigger mode not recognized"));
         break;
   }

   return;
}

void Gui::postTrigPercentageSlot(int value) {

   m_postTrigPercentage = value;
   return;
}

void Gui::highImpedanceGPOSlot(bool checked) {

   m_highImpedanceGPO = checked;
   return;
}

void Gui::signalTypeSlot(int index) {

   // Translate the state of the combo box into an enumeration value:
   switch (index) {

      case 0:
         m_signalType = SGNL_NIM;
         break;
      case 1:
         m_signalType = SGNL_TTL;
         break;
      default:
         REPORT_ERROR(tr("Front panel signal type not recognized"));
         break;
   }

   return;
}

void Gui::patGenEnabledSlot(bool checked) {

   m_patGenEnabled = checked;
   return;
}

void Gui::saveRawNtupleSlot(bool checked) {

   m_saveRawNtuple = checked;
   return;
}

void Gui::acqModeSlot(int index) {

   // Translate the state of the combo box into an enumeration value:
   switch (index) {

      case 0:
         m_acqMode = caen::Digitizer::ACQ_SW_Controlled;
         break;
      case 1:
         m_acqMode = caen::Digitizer::ACQ_SIn_Controlled;
         ;
         break;
      default:
         REPORT_ERROR(tr("Acquisition mode not recognized"));
         break;
   }

   return;
}

void Gui::gateModeSlot(int index) {

   // Translate the state of the combo box into an enumeration value:
   switch (index) {

      case 0:
         m_gateMode = GATE_Window;
         break;
      case 1:
         m_gateMode = GATE_SingleShot;
         break;
      default:
         REPORT_ERROR(tr("Gate mode not recognized"));
         break;
   }

   return;
}

void Gui::bufferModeSlot(int index) {

   // Translate the state of the combo box into an enumeration value:
   switch (index) {

      case 0:
         m_bufferMode = BUFF_NBuffers1;
         break;
      case 1:
         m_bufferMode = BUFF_NBuffers2;
         break;
      case 2:
         m_bufferMode = BUFF_NBuffers4;
         break;
      case 3:
         m_bufferMode = BUFF_NBuffers8;
         break;
      case 4:
         m_bufferMode = BUFF_NBuffers16;
         break;
      case 5:
         m_bufferMode = BUFF_NBuffers32;
         break;
      case 6:
         m_bufferMode = BUFF_NBuffers64;
         break;
      case 7:
         m_bufferMode = BUFF_NBuffers128;
         break;
      case 8:
         m_bufferMode = BUFF_NBuffers256;
         break;
      case 9:
         m_bufferMode = BUFF_NBuffers512;
         break;
      case 10:
         m_bufferMode = BUFF_NBuffers1024;
         break;
      default:
         REPORT_ERROR(tr("Buffer mode not recognized"));
         break;
   }

   return;
}

void Gui::clockSourceSlot(int index) {

   switch (index) {

      case 0:
         m_clockSource = CLK_Internal;
         break;
      case 1:
         m_clockSource = CLK_External;
         break;
      default:
         REPORT_ERROR(tr("Clock source not recognized"));
         break;
   }

   return;
}

void Gui::evCountModeSlot(int index) {

   switch (index) {

      case 0:
         m_evCountMode = EV_CountAcceptedTriggers;
         break;
      case 1:
         m_evCountMode = EV_CountAllTriggers;
         break;
      default:
         REPORT_ERROR(tr("Event counting mode not recognized"));
         break;
   }

   return;
}

void Gui::cfdFractionSlot(double value) {

   m_cfdFraction = value;
   return;
}

void Gui::cfdDelaySlot(int value) {

   m_cfdDelay = value;
   return;
}

void Gui::cfdLengthSlot(int value) {

   m_cfdLength = value;
   return;
}

void Gui::gaussSmoothWidthSlot(double value) {

   m_gaussSmoothWidth = value;
   return;
}

/**
 * After a configuration is read from file, the graphical objects have
 * to be synced to show this new configuration. This function takes
 * care of doing that.
 */
void Gui::sync() {

   // Set the connection mode:
   m_connMode->setEnabled(false);
   switch (m_connType) {
      case caen::Digitizer::USB:
         m_connMode->setCurrentIndex(0);
         break;
      case caen::Digitizer::PCI_OpticalLink:
         m_connMode->setCurrentIndex(1);
         break;
      case caen::Digitizer::PCIE_OpticalLink:
         m_connMode->setCurrentIndex(2);
         break;
      case caen::Digitizer::PCIE_EmbeddedDigitizer:
         m_connMode->setCurrentIndex(3);
         break;
      default:
         REPORT_ERROR(tr("Connection mode not recognized"));
         break;
   }
   m_connMode->setEnabled(true);

   // Set the connection link:
   m_connLink->setEnabled(false);
   m_connLink->setValue(m_linkNumber);
   m_connLink->setEnabled(true);

   // Set the trigger overlap:
   m_trigOvlpEnabledWidget->setEnabled(false);
   m_trigOvlpEnabledWidget->setChecked(m_trigOvlpEnabled);
   m_trigOvlpEnabledWidget->setEnabled(true);

   // Set the external trigger:
   m_extTrigEnabledWidget->setEnabled(false);
   m_extTrigEnabledWidget->setChecked(m_extTrigEnabled);
   m_extTrigEnabledWidget->setEnabled(true);

   // Set the external trigger forwarding:
   m_extTrigOutEnabledWidget->setEnabled(false);
   m_extTrigOutEnabledWidget->setChecked(m_extTrigOutEnabled);
   m_extTrigOutEnabledWidget->setEnabled(true);

   // Set the high impedance GPO:
   m_highImpedanceGPOWidget->setEnabled(false);
   m_highImpedanceGPOWidget->setChecked(m_highImpedanceGPO);
   m_highImpedanceGPOWidget->setEnabled(true);

   // Set the percentage of post trigger samples:
   m_postTrigPercentageWidget->setEnabled(false);
   m_postTrigPercentageWidget->setValue(m_postTrigPercentage);
   m_postTrigPercentageWidget->setEnabled(true);

   // Set the trigger mode:
   m_trigModeWidget->setEnabled(false);
   switch (m_trigMode) {

      case TRG_InputOverThreshold:
         m_trigModeWidget->setCurrentIndex(0);
         break;
      case TRG_InputUnderThreshold:
         m_trigModeWidget->setCurrentIndex(1);
         break;
      default:
         REPORT_ERROR(tr("Trigger mode not recognized"));
         break;
   }
   m_trigModeWidget->setEnabled(true);

   // Set the front panel signal type:
   m_signalTypeWidget->setEnabled(false);
   switch (m_signalType) {

      case SGNL_NIM:
         m_signalTypeWidget->setCurrentIndex(0);
         break;
      case SGNL_TTL:
         m_signalTypeWidget->setCurrentIndex(1);
         break;
      default:
         REPORT_ERROR(tr("Signal type not recognized"));
         break;
   }
   m_signalTypeWidget->setEnabled(true);

   // Set the state of the pattern generation:
   m_patGenEnabledWidget->setEnabled(false);
   m_patGenEnabledWidget->setChecked(m_patGenEnabled);
   m_patGenEnabledWidget->setEnabled(true);

   // Set the state of the raw ntuple writing:
   m_saveRawNtupleWidget->setEnabled(false);
   m_saveRawNtupleWidget->setChecked(m_saveRawNtuple);
   m_saveRawNtupleWidget->setEnabled(true);

   // Set the acquisition mode:
   m_acqModeWidget->setEnabled(false);
   switch (m_acqMode) {

      case caen::Digitizer::ACQ_SW_Controlled:
         m_acqModeWidget->setCurrentIndex(0);
         break;
      case caen::Digitizer::ACQ_SIn_Controlled:
         m_acqModeWidget->setCurrentIndex(1);
         break;
      default:
         REPORT_ERROR(tr("Acquisition mode not recognized"));
         break;
   }
   m_acqModeWidget->setEnabled(true);

   // Set the gate mode:
   m_gateModeWidget->setEnabled(false);
   switch (m_gateMode) {

      case GATE_Window:
         m_gateModeWidget->setCurrentIndex(0);
         break;
      case GATE_SingleShot:
         m_gateModeWidget->setCurrentIndex(1);
         break;
      default:
         REPORT_ERROR(tr("Gate mode not recognized"));
         break;
   }
   m_gateModeWidget->setEnabled(true);

   // Set the buffer mode:
   m_bufferModeWidget->setEnabled(false);
   switch (m_bufferMode) {

      case BUFF_NBuffers1:
         m_bufferModeWidget->setCurrentIndex(0);
         break;
      case BUFF_NBuffers2:
         m_bufferModeWidget->setCurrentIndex(1);
         break;
      case BUFF_NBuffers4:
         m_bufferModeWidget->setCurrentIndex(2);
         break;
      case BUFF_NBuffers8:
         m_bufferModeWidget->setCurrentIndex(3);
         break;
      case BUFF_NBuffers16:
         m_bufferModeWidget->setCurrentIndex(4);
         break;
      case BUFF_NBuffers32:
         m_bufferModeWidget->setCurrentIndex(5);
         break;
      case BUFF_NBuffers64:
         m_bufferModeWidget->setCurrentIndex(6);
         break;
      case BUFF_NBuffers128:
         m_bufferModeWidget->setCurrentIndex(7);
         break;
      case BUFF_NBuffers256:
         m_bufferModeWidget->setCurrentIndex(8);
         break;
      case BUFF_NBuffers512:
         m_bufferModeWidget->setCurrentIndex(9);
         break;
      case BUFF_NBuffers1024:
         m_bufferModeWidget->setCurrentIndex(10);
         break;
      default:
         REPORT_ERROR(tr("Buffer mode not recognized"));
         break;
   }
   m_bufferModeWidget->setEnabled(true);

   // Set the clock source:
   m_clockSourceWidget->setEnabled(false);
   switch (m_clockSource) {

      case CLK_Internal:
         m_clockSourceWidget->setCurrentIndex(0);
         break;
      case CLK_External:
         m_clockSourceWidget->setCurrentIndex(1);
         break;
      default:
         REPORT_ERROR(tr("Clock source not recognized"));
         break;
   }
   m_clockSourceWidget->setEnabled(true);

   // Set the event counting mode:
   m_evCountModeWidget->setEnabled(false);
   switch (m_evCountMode) {

      case EV_CountAcceptedTriggers:
         m_evCountModeWidget->setCurrentIndex(0);
         break;
      case EV_CountAllTriggers:
         m_evCountModeWidget->setCurrentIndex(1);
         break;
      default:
         REPORT_ERROR(tr("Event counting mode not recognized"));
         break;
   }
   m_evCountModeWidget->setEnabled(true);

   // Set the CFD fraction:
   m_cfdFractionWidget->setEnabled(false);
   m_cfdFractionWidget->setValue(m_cfdFraction);
   m_cfdFractionWidget->setEnabled(true);

   // Set the CFD delay:
   m_cfdDelayWidget->setEnabled(false);
   m_cfdDelayWidget->setValue(m_cfdDelay);
   m_cfdDelayWidget->setEnabled(true);

   // Set the CFD length:
   m_cfdLengthWidget->setEnabled(false);
   m_cfdLengthWidget->setValue(m_cfdLength);
   m_cfdLengthWidget->setEnabled(true);

   // Set the Gaussian smoothing width:
   m_gaussSmoothWidthWidget->setEnabled(false);
   m_gaussSmoothWidthWidget->setValue(m_gaussSmoothWidth);
   m_gaussSmoothWidthWidget->setEnabled(true);

   // Synchronize all the groups:
   for (int i = 0; i < NUMBER_OF_GROUPS; ++i) {
      m_ggroups[i]->sync();
   }

   return;
}

}  // namespace dt5740
