//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// Local include(s).
#include "CamacReaderRunner.h"

// CDA include(s).
#include "common/AppRunner.h"
#include "common/PathResolver.h"

// Qt include(s).
#include <QApplication>
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QPalette>
#include <QPushButton>
#include <QTime>
#include <QVBoxLayout>

// System include(s).
#include <set>

namespace simple_daq {

struct CamacReaderRunner::Impl {

   /// Constructor
   Impl(QWidget& parent, const QString& boxTitle, const QString& startLabelText,
        const QString& startText, const QString& stopLabelText,
        const QString& stopText, const QString& buttonText,
        const QString& processStatusText)
       : m_mainBox{boxTitle, &parent},
         m_layout{&m_mainBox},
         m_processStatus{processStatusText, &m_mainBox},
         m_startTimeLabel{startLabelText, &m_mainBox},
         m_startTime{startText, &m_mainBox},
         m_stopTimeLabel{stopLabelText, &m_mainBox},
         m_stopTime{stopText, &m_mainBox},
         m_starterButton{QIcon::fromTheme("media-playback-start"), buttonText,
                         &m_mainBox} {

      //
      // Lay out the widgets.
      //
      m_mainLayout.addWidget(&m_mainBox);
      parent.setLayout(&m_mainLayout);

      m_layout.addWidget(&m_processStatus, 0, 0, 1, 2, Qt::AlignCenter);
      m_layout.addWidget(&m_startTimeLabel, 1, 0,
                         Qt::AlignRight | Qt::AlignVCenter);
      m_layout.addWidget(&m_startTime, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
      m_layout.addWidget(&m_stopTimeLabel, 2, 0,
                         Qt::AlignRight | Qt::AlignVCenter);
      m_layout.addWidget(&m_stopTime, 2, 1, Qt::AlignLeft | Qt::AlignVCenter);
      m_layout.addWidget(&m_starterButton, 3, 0, 1, 2);
   }

   /// Layout connecting the parent to the main box
   QVBoxLayout m_mainLayout;

   /// Main box, holding other graphcal objects
   QGroupBox m_mainBox;

   /// Layout for the elements of the main box.
   QGridLayout m_layout;

   /// Label giving feedback of the status
   QLabel m_processStatus;

   /// "Start time" label
   QLabel m_startTimeLabel;
   /// Label with the starting time
   QLabel m_startTime;
   /// "Stop time" label
   QLabel m_stopTimeLabel;
   /// Label with the stopping time
   QLabel m_stopTime;

   /// Button starting the application
   QPushButton m_starterButton;

   /// Name of the configuration file
   QString m_configFileName;
   /// Address of the message server
   QString m_msgServerAddress;
   /// Address(es) of the statistics server(s)
   std::set<QString> m_statServerAddresses;
   /// Addresses of event listener applications
   std::set<QString> m_eventListenerAddresses;
   /// Output level of cda-caen-vme-reader
   msg::Level m_level;

   /// The object starting the application
   daq::AppRunner m_runner;
};

CamacReaderRunner::CamacReaderRunner(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags),
      m_impl{new Impl{*this, tr("Camac acquisition control"), tr("Start time:"),
                      tr("N/A"), tr("Stop time:"), tr("N/A"),
                      tr("Start camac reader"), tr("Camac reader stopped")}} {

   //
   // Set up the process status label.
   //
   QPalette palette(m_impl->m_processStatus.palette());
   palette.setColor(QPalette::Active, QPalette::WindowText,
                    QColor(10, 150, 10));
   palette.setColor(QPalette::Inactive, QPalette::WindowText,
                    QColor(10, 150, 10));
   m_impl->m_processStatus.setPalette(palette);

   QFont font(m_impl->m_processStatus.font());
   font.setPointSize(14);
   m_impl->m_processStatus.setFont(font);

   //
   // Create the button starting and stopping the application:
   //
   m_impl->m_starterButton.setCheckable(true);
   connect(&(m_impl->m_starterButton), SIGNAL(clicked(bool)), this,
           SLOT(startApp(bool)));

   //
   // Configure the application runner object:
   //
   m_impl->m_runner.setExecName(
       daq::PathResolver::resolve("cda-camac-reader", "PATH"));
   if (m_impl->m_runner.getExecName() == "") {
      QMessageBox::critical(this, tr("Error"),
                            tr("cda-camac-reader not found. Data "
                               "acquisition is not possible."));
      m_impl->m_mainBox.setEnabled(false);
   }
}

CamacReaderRunner::~CamacReaderRunner() {}

/**
 * @param fileName The name of the configuration file to be used
 */
void CamacReaderRunner::setConfigFileName(const QString& fileName) {

   m_impl->m_configFileName = fileName;
}

/**
 * @returns The name of the configuration file to be used
 */
const QString& CamacReaderRunner::getConfigFileName() const {

   return m_impl->m_configFileName;
}

/**
 * @param address The message server address to be used by the application
 */
void CamacReaderRunner::setMsgServerAddress(const QString& address) {

   m_impl->m_msgServerAddress = address;
}

/**
 * @returns The message server address to be used by the application
 */
const QString& CamacReaderRunner::getMsgServerAddress() const {

   return m_impl->m_msgServerAddress;
}

/**
 * @param verbosity The output verbosity of cda-camac-reader
 */
void CamacReaderRunner::setVerbosity(msg::Level verbosity) {

   m_impl->m_level = verbosity;
}

/**
 * @returns The output verbosity of cda-camac-reader
 */
msg::Level CamacReaderRunner::getVerbosity() const {

   return m_impl->m_level;
}

/**
 * @param status The required state for the widget
 */
void CamacReaderRunner::setEnabled(bool status) {

   // Always allow disabling the widgets:
   if (!status) {
      m_impl->m_mainBox.setEnabled(status);
   }
   // Only enable the widgets if cda-glomem-writer has been found:
   else if (m_impl->m_runner.getExecName() != "") {
      m_impl->m_mainBox.setEnabled(status);
   }
}

void CamacReaderRunner::setWriterRunning(bool running, const QString& address) {

   // Update the running applications flag:
   if (running) {
      m_impl->m_eventListenerAddresses.insert(address);
   } else {
      m_impl->m_eventListenerAddresses.erase(address);
   }
}

/**
 * @param status Selects whether the address should be added or removed
 * @param address The statistics server address to be used by the application
 */
void CamacReaderRunner::setStatServerAddress(bool status,
                                             const QString& address) {

   if (status) {
      m_impl->m_statServerAddresses.insert(address);
   } else {
      m_impl->m_statServerAddresses.erase(address);
   }
}

/**
 * This is the function doing most of the work in this class. It puts the
 * command line options together to start cda-camac-reader correctly, then
 * it starts the application in a new process.
 *
 * @param start <code>true</code> if the application should be started,
 *              <code>false</code> if it should be stopped
 */
void CamacReaderRunner::startApp(bool start) {

   if (start) {

      //
      // Collect the trivial command line options:
      //
      QString options;
      options += " -m " + m_impl->m_msgServerAddress;
      options += " -c " + m_impl->m_configFileName;
      options += " -v " + QString::number(m_impl->m_level);

      //
      // Collect where the application should send statistics information to:
      //
      for (const QString& server : m_impl->m_statServerAddresses) {
         options += " -s " + server;
      }

      //
      // Collect where the application should send events to:
      //
      for (const QString& server : m_impl->m_eventListenerAddresses) {
         options += " -e " + server;
      }

      //
      // Set these options to the runner object:
      //
      m_impl->m_runner.setOptions(options);

      if (!m_impl->m_runner.start()) {

         //
         // Signal the error as much as possible:
         //
         m_impl->m_processStatus.setText(tr("ERROR"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(150, 10, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(150, 10, 10));
         m_impl->m_processStatus.setPalette(palette);

         m_impl->m_starterButton.setText(tr("Reset"));
         m_impl->m_starterButton.setIcon(QIcon::fromTheme("edit-clear"));

      } else {

         //
         // Signal that the application is running:
         //
         m_impl->m_processStatus.setText(tr("Camac reader running"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(150, 10, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(150, 10, 10));
         m_impl->m_processStatus.setPalette(palette);

         // Set up the time label(s):
         m_impl->m_startTime.setText(QTime::currentTime().toString());
         m_impl->m_stopTime.setText(tr("N/A"));

         m_impl->m_starterButton.setText(tr("Stop camac reader"));
         m_impl->m_starterButton.setIcon(
             QIcon::fromTheme("media-playback-stop"));
      }

   } else {

      if (!m_impl->m_runner.stop()) {

         //
         // Signal the error as much as possible:
         //
         m_impl->m_processStatus.setText(tr("ERROR"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(150, 10, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(150, 10, 10));
         m_impl->m_processStatus.setPalette(palette);

      } else {

         //
         // Return the widgets to their "normal" state:
         //
         m_impl->m_processStatus.setText(tr("Camac reader stopped"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(10, 150, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(10, 150, 10));
         m_impl->m_processStatus.setPalette(palette);

         // Set up the time label(s):
         m_impl->m_stopTime.setText(QTime::currentTime().toString());
      }

      m_impl->m_starterButton.setText(tr("Start camac reader"));
      m_impl->m_starterButton.setIcon(QIcon::fromTheme("media-playback-start"));
   }
}

}  // namespace simple_daq
