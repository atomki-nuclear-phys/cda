//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// Local include(s):
#include "RawWriterRunner.h"

// CDA include(s):
#include "common/AppRunner.h"
#include "common/PathResolver.h"

// Qt include(s):
#include <QFont>
#include <QGridLayout>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QPalette>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

// System include(s).
#include <set>

namespace simple_daq {

struct RawWriterRunner::Impl {

   /// Constructor
   Impl(QWidget& parent, const QString& boxTitle, const QString& labelText,
        const QString& fileNameText, const QString& buttonText)
       : m_mainBox{boxTitle, &parent},
         m_layout{&m_mainBox},
         m_processStatus{labelText, &m_mainBox},
         m_fileNameLabel{fileNameText, &m_mainBox},
         m_fileNameEdit{&m_mainBox},
         m_starterButton{QIcon::fromTheme("media-playback-start"), buttonText,
                         &m_mainBox} {

      m_mainLayout.addWidget(&m_mainBox);
      parent.setLayout(&m_mainLayout);

      m_layout.addWidget(&m_processStatus, 0, 0, 1, 2, Qt::AlignCenter);
      m_layout.addWidget(&m_fileNameLabel, 1, 0, Qt::AlignRight);
      m_layout.addWidget(&m_fileNameEdit, 1, 1);
      m_layout.addWidget(&m_starterButton, 2, 0, 1, 2);
   }

   /// Layout connecting the parent to the main box
   QVBoxLayout m_mainLayout;

   /// Main box holding graphcal objects
   QGroupBox m_mainBox;

   /// Layout for the elements of the main box.
   QGridLayout m_layout;

   /// Label giving feedback of the status
   QLabel m_processStatus;
   /// Label telling what the line edit is for
   QLabel m_fileNameLabel;
   /// Input field for the output file name
   QLineEdit m_fileNameEdit;
   /// Button starting the application
   QPushButton m_starterButton;

   /// Name of the configuration file
   QString m_configFileName;
   /// Address of the message server(s)
   QString m_msgServerAddress;
   /// Address of cda-hbook-writer
   QString m_eventAddress;
   /// Address(es) of the statistics server(s)
   std::set<QString> m_statServerAddresses;
   /// Output level of cda-hbook-writer
   msg::Level m_level;

   /// The object starting cda-hbook-writer
   daq::AppRunner m_runner;
};

RawWriterRunner::RawWriterRunner(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags),
      m_impl{new Impl{*this, tr("Raw file writing control"),
                      tr("Raw writer stopped"), tr("Output file:"),
                      tr("Start raw writer")}} {

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
   // Set up the button starting the application:
   //
   m_impl->m_starterButton.setCheckable(true);
   connect(&(m_impl->m_starterButton), SIGNAL(clicked(bool)), this,
           SLOT(startApp(bool)));

   m_impl->m_runner.setExecName(
       daq::PathResolver::resolve("cda-raw-writer", "PATH"));
   if (m_impl->m_runner.getExecName() == "") {
      m_impl->m_mainBox.setEnabled(false);
   }
}

RawWriterRunner::~RawWriterRunner() {}

void RawWriterRunner::setConfigFileName(const QString& fileName) {

   m_impl->m_configFileName = fileName;
}

const QString& RawWriterRunner::getConfigFileName() const {

   return m_impl->m_configFileName;
}

void RawWriterRunner::setMsgServerAddress(const QString& address) {

   m_impl->m_msgServerAddress = address;
}

const QString& RawWriterRunner::getMsgServerAddress() const {

   return m_impl->m_msgServerAddress;
}

void RawWriterRunner::setEventAddress(const QString& address) {

   m_impl->m_eventAddress = address;
}

const QString& RawWriterRunner::getEventAddress() const {

   return m_impl->m_eventAddress;
}

void RawWriterRunner::setVerbosity(msg::Level verbosity) {

   m_impl->m_level = verbosity;
}

msg::Level RawWriterRunner::getVerbosity() const {

   return m_impl->m_level;
}

/**
 * @param status The required state for the widget
 */
void RawWriterRunner::setEnabled(bool status) {

   // Always allow disabling the widgets:
   if (!status) {
      m_impl->m_mainBox.setEnabled(status);
   }
   // Only enable the widgets if cda-glomem-writer has been found:
   else if (m_impl->m_runner.getExecName() != "") {
      m_impl->m_mainBox.setEnabled(status);
   }
}

/**
 * @param status Selects whether the address should be added or removed
 * @param address The statistics server address to be used by the application
 */
void RawWriterRunner::setStatServerAddress(bool status,
                                           const QString& address) {

   if (status) {
      m_impl->m_statServerAddresses.insert(address);
   } else {
      m_impl->m_statServerAddresses.erase(address);
   }
}

void RawWriterRunner::startApp(bool start) {

   if (start) {

      //
      // Set up some things for the automatic file name updating:
      //
      m_impl->m_fileNameEdit.setReadOnly(true);

      //
      // Construct the application arguments:
      //
      QString options;
      options += " -m " + m_impl->m_msgServerAddress;
      options += " -c " + m_impl->m_configFileName;
      options += " -v " + QString::number(m_impl->m_level);
      options += " -e " + m_impl->m_eventAddress;
      options += " -o " + m_impl->m_fileNameEdit.text();

      //
      // Collect where the application should send statistics information to:
      //
      for (const QString& server : m_impl->m_statServerAddresses) {
         options += " -s " + server;
      }

      m_impl->m_runner.setOptions(options);

      if (!m_impl->m_runner.start()) {

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

         m_impl->m_processStatus.setText(tr("Raw writer running"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(150, 10, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(150, 10, 10));
         m_impl->m_processStatus.setPalette(palette);

         m_impl->m_starterButton.setText(tr("Stop raw writer"));
         m_impl->m_starterButton.setIcon(
             QIcon::fromTheme("media-playback-stop"));

         emit running(true);
         emit receiverRunning(true, m_impl->m_eventAddress);
      }

   } else {

      if (!m_impl->m_runner.stop()) {

         m_impl->m_processStatus.setText(tr("ERROR"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(150, 10, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(150, 10, 10));
         m_impl->m_processStatus.setPalette(palette);

      } else {

         m_impl->m_processStatus.setText(tr("Raw writer stopped"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(10, 150, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(10, 150, 10));
         m_impl->m_processStatus.setPalette(palette);
      }

      emit running(false);
      emit receiverRunning(false, m_impl->m_eventAddress);
      m_impl->m_starterButton.setText(tr("Start raw writer"));
      m_impl->m_starterButton.setIcon(QIcon::fromTheme("media-playback-start"));

      m_impl->m_fileNameEdit.setReadOnly(false);
   }
}

}  // namespace simple_daq
