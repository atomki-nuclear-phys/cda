//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// Local include(s).
#include "GlomemWriterRunner.h"

// CDA include(s).
#include "common/AppRunner.h"
#include "common/PathResolver.h"

// Qt include(s).
#include <QFont>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>

// System include(s).
#include <set>

namespace simple_daq {

struct GlomemWriterRunner::Impl {

   /// Constructor
   Impl(QWidget& parent, const QString& boxTitle, const QString& labelText,
        const QString& buttonText)
       : m_mainBox{boxTitle, &parent},
         m_layout{&m_mainBox},
         m_processStatus{labelText, &m_mainBox},
         m_starterButton{QIcon::fromTheme("media-playback-start"), buttonText,
                         &m_mainBox} {

      m_mainLayout.addWidget(&m_mainBox);
      parent.setLayout(&m_mainLayout);

      m_layout.addWidget(&m_processStatus, 1, Qt::AlignCenter);
      m_layout.addWidget(&m_starterButton, 0);
   }

   /// Layout connecting the parent to the main box
   QVBoxLayout m_mainLayout;

   /// Main box holding graphcal objects
   QGroupBox m_mainBox;

   /// Layout for the elements of the main box.
   QVBoxLayout m_layout;

   /// Label giving feedback of the status
   QLabel m_processStatus;
   /// Button starting the application
   QPushButton m_starterButton;

   /// Name of the configuration file
   QString m_configFileName;
   /// Address of the message server(s)
   QString m_msgServerAddress;
   /// Address of cda-glomem-writer
   QString m_eventAddress;
   /// Address(es) of the statistics server(s)
   std::set<QString> m_statServerAddresses;
   /// Output level of cda-glomem-writer
   msg::Level m_level;

   /// The object starting cda-glomem-writer
   daq::AppRunner m_runner;
};

GlomemWriterRunner::GlomemWriterRunner(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags),
      m_impl{new Impl{*this, tr("GloMem writing control"),
                      tr("GloMem writer stopped"), tr("Start glomem writer")}} {

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
   // Set up the starter button.
   //
   m_impl->m_starterButton.setCheckable(true);
   connect(&(m_impl->m_starterButton), SIGNAL(clicked(bool)), this,
           SLOT(startApp(bool)));

   //
   // Configure the application runner object.
   //
   m_impl->m_runner.setExecName(
       daq::PathResolver::resolve("cda-glomem-writer", "PATH"));
   if (m_impl->m_runner.getExecName() == "") {
      m_impl->m_mainBox.setEnabled(false);
   }
}

GlomemWriterRunner::~GlomemWriterRunner() {}

void GlomemWriterRunner::setConfigFileName(const QString& fileName) {

   m_impl->m_configFileName = fileName;
}

const QString& GlomemWriterRunner::getConfigFileName() const {

   return m_impl->m_configFileName;
}

void GlomemWriterRunner::setMsgServerAddress(const QString& address) {

   m_impl->m_msgServerAddress = address;
}

const QString& GlomemWriterRunner::getMsgServerAddress() const {

   return m_impl->m_msgServerAddress;
}

void GlomemWriterRunner::setEventAddress(const QString& address) {

   m_impl->m_eventAddress = address;
}

const QString& GlomemWriterRunner::getEventAddress() const {

   return m_impl->m_eventAddress;
}

void GlomemWriterRunner::setVerbosity(msg::Level verbosity) {

   m_impl->m_level = verbosity;
}

msg::Level GlomemWriterRunner::getVerbosity() const {

   return m_impl->m_level;
}

/**
 * @param status The required state for the widget
 */
void GlomemWriterRunner::setEnabled(bool status) {

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
void GlomemWriterRunner::setStatServerAddress(bool status,
                                              const QString& address) {

   if (status) {
      m_impl->m_statServerAddresses.insert(address);
   } else {
      m_impl->m_statServerAddresses.erase(address);
   }
}

void GlomemWriterRunner::startApp(bool start) {

   if (start) {

      QString options;
      options += " -m " + m_impl->m_msgServerAddress;
      options += " -c " + m_impl->m_configFileName;
      options += " -v " + QString::number(m_impl->m_level);
      options += " -e " + m_impl->m_eventAddress;

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

         m_impl->m_processStatus.setText(tr("GloMem writer running"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(150, 10, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(150, 10, 10));
         m_impl->m_processStatus.setPalette(palette);

         m_impl->m_starterButton.setText(tr("Stop glomem writer"));
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

         m_impl->m_processStatus.setText(tr("GloMem writer stopped"));
         QPalette palette(m_impl->m_processStatus.palette());
         palette.setColor(QPalette::Active, QPalette::WindowText,
                          QColor(10, 150, 10));
         palette.setColor(QPalette::Inactive, QPalette::WindowText,
                          QColor(10, 150, 10));
         m_impl->m_processStatus.setPalette(palette);
      }

      emit running(false);
      emit receiverRunning(false, m_impl->m_eventAddress);
      m_impl->m_starterButton.setText(tr("Start glomem writer"));
      m_impl->m_starterButton.setIcon(QIcon::fromTheme("media-playback-start"));
   }
}

}  // namespace simple_daq
