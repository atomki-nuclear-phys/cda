//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2025 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// Local include(s).
#include "MainWindow.h"

#include "Constants.h"

// CDA include(s).
#include "common/Address.h"
#include "common/DefaultFont.h"
#include "common/aboutCDA.h"

// Qt include(s).
#include <QApplication>
#include <QFileDialog>
#include <QIcon>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>

/**
 * The constructor takes care of drawing the window, and setting up the
 * interaction between the various Qt objects.
 *
 * @param confFileName Name of the configuration file to be used
 * @param verbosity Output message verbosity of all components
 */
MainWindow::MainWindow(const QString& confFileName, msg::Level verbosity)
    : QMainWindow(),
      m_centralWidget{this},
      m_layout{&m_centralWidget},
      m_msgView{&m_centralWidget},
      m_statistics{&m_centralWidget},
      m_caenReader{&m_centralWidget},
      m_glomemWriter{&m_centralWidget},
      m_hbookWriter{&m_centralWidget},
      m_rootWriter{&m_centralWidget},
      m_rawWriter{&m_centralWidget},
      m_eventReceiversLabel{tr("Extra event receivers:"), &m_centralWidget},
      m_eventReceivers{&m_centralWidget},
      m_removeEventReceiver{QIcon::fromTheme("list-remove"), tr("Remove"),
                            &m_centralWidget},
      m_statReceiversLabel{tr("Extra statistics receivers:"), &m_centralWidget},
      m_statReceivers{&m_centralWidget},
      m_removeStatReceiver{QIcon::fromTheme("list-remove"), tr("Remove"),
                           &m_centralWidget} {

   // Set up the window according to whether a configuration file was already
   // specified:
   if (!confFileName.isEmpty()) {
      setWindowTitle(tr("CDA CAEN Digitizer DAQ - %1").arg(confFileName));
   } else {
      setWindowTitle(tr("CDA CAEN Digitizer DAQ"));
   }
   setWindowIcon(QIcon(":/img/caen.png"));

   // Set the central widget.
   setCentralWidget(&m_centralWidget);

   //
   // Set up the message server and the widget showing the incoming messages:
   //
   m_msgServer.setWriteOutputFile(false);
   m_layout.addWidget(&m_msgView, 4, 0, 1, 3);
   m_msgView.setMinimumShownLevel(verbosity);
   connect(&m_msgServer, SIGNAL(messageAvailable(const Message&)), &m_msgView,
           SLOT(addMessage(const Message&)));
   if (!m_msgServer.listen(Address(MSG_SERVER_ADDRESS))) {
      QMessageBox::critical(this, tr("Error starting message server"),
                            tr("The TCP/IP message server could not be "
                               "started on address %1. The application has "
                               "to shut down!")
                                .arg(MSG_SERVER_ADDRESS));
      QApplication::exit(1);
   }

   //
   // Set up the "simple_daq" widgets.
   //
   m_layout.addWidget(&m_statistics, 0, 0);
   m_statistics.setStatAddress(Address{STAT_SERVER_ADDRESS});
   m_statistics.setEnabled(!confFileName.isEmpty());

   m_layout.addWidget(&m_caenReader, 0, 1);
   m_caenReader.setConfigFileName(confFileName);
   m_caenReader.setMsgServerAddress(MSG_SERVER_ADDRESS);
   m_caenReader.setStatServerAddress(true, STAT_SERVER_ADDRESS);
   m_caenReader.setVerbosity(verbosity);
   m_caenReader.setEnabled(!confFileName.isEmpty());

   m_layout.addWidget(&m_glomemWriter, 0, 2);
   m_glomemWriter.setConfigFileName(confFileName);
   m_glomemWriter.setMsgServerAddress(MSG_SERVER_ADDRESS);
   m_glomemWriter.setEventAddress(GLOMEM_WRITER_ADDRESS);
   m_glomemWriter.setVerbosity(verbosity);
   m_glomemWriter.setEnabled(!confFileName.isEmpty());
   connect(&m_glomemWriter, SIGNAL(receiverRunning(bool, const QString&)),
           &m_caenReader, SLOT(setWriterRunning(bool, const QString&)));

   m_layout.addWidget(&m_hbookWriter, 1, 0);
   m_hbookWriter.setConfigFileName(confFileName);
   m_hbookWriter.setMsgServerAddress(MSG_SERVER_ADDRESS);
   m_hbookWriter.setEventAddress(HBOOK_WRITER_ADDRESS);
   m_hbookWriter.setVerbosity(verbosity);
   m_hbookWriter.setEnabled(!confFileName.isEmpty());
   connect(&m_hbookWriter, SIGNAL(receiverRunning(bool, const QString&)),
           &m_caenReader, SLOT(setWriterRunning(bool, const QString&)));

   m_layout.addWidget(&m_rootWriter, 1, 1);
   m_rootWriter.setConfigFileName(confFileName);
   m_rootWriter.setMsgServerAddress(MSG_SERVER_ADDRESS);
   m_rootWriter.setEventAddress(ROOT_WRITER_ADDRESS);
   m_rootWriter.setVerbosity(verbosity);
   m_rootWriter.setEnabled(!confFileName.isEmpty());
   connect(&m_rootWriter, SIGNAL(receiverRunning(bool, const QString&)),
           &m_caenReader, SLOT(setWriterRunning(bool, const QString&)));

   m_layout.addWidget(&m_rawWriter, 1, 2);
   m_rawWriter.setConfigFileName(confFileName);
   m_rawWriter.setMsgServerAddress(MSG_SERVER_ADDRESS);
   m_rawWriter.setEventAddress(RAW_WRITER_ADDRESS);
   m_rawWriter.setVerbosity(verbosity);
   m_rawWriter.setEnabled(!confFileName.isEmpty());
   connect(&m_rawWriter, SIGNAL(receiverRunning(bool, const QString&)),
           &m_caenReader, SLOT(setWriterRunning(bool, const QString&)));

   //
   // Create the widgets for specifying extra event receivers.
   //
   m_layout.addWidget(&m_eventReceiversLabel, 2, 0);
   m_eventReceiversLabel.setAlignment(Qt::AlignRight);
   m_eventReceiversLabel.setEnabled(!confFileName.isEmpty());

   m_layout.addWidget(&m_eventReceivers, 2, 1);
   m_eventReceivers.setEditable(true);
   m_eventReceivers.setDuplicatesEnabled(false);
   m_eventReceivers.setEnabled(!confFileName.isEmpty());
   connect(&m_eventReceivers, SIGNAL(currentIndexChanged(const QString&)), this,
           SLOT(eventReceiverIndexChangedSlot(const QString&)));
   connect(&m_eventReceivers, SIGNAL(editTextChanged(const QString&)), this,
           SLOT(eventReceiverTextChangedSlot(const QString&)));

   m_currentEventReceiver = m_eventReceivers.currentText();

   m_layout.addWidget(&m_removeEventReceiver, 2, 2, 1, 1, Qt::AlignLeft);
   m_removeEventReceiver.setEnabled(!confFileName.isEmpty());
   connect(&m_removeEventReceiver, SIGNAL(pressed()), this,
           SLOT(removeEventReceiverSlot()));

   //
   // Create the widgets for specifying extra statistics receivers.
   //
   m_layout.addWidget(&m_statReceiversLabel, 3, 0);
   m_statReceiversLabel.setAlignment(Qt::AlignRight);
   m_statReceiversLabel.setEnabled(!confFileName.isEmpty());

   m_layout.addWidget(&m_statReceivers, 3, 1);
   m_statReceivers.setEditable(true);
   m_statReceivers.setDuplicatesEnabled(false);
   m_statReceivers.setEnabled(!confFileName.isEmpty());
   connect(&m_statReceivers, SIGNAL(currentIndexChanged(const QString&)), this,
           SLOT(statReceiverIndexChangedSlot(const QString&)));
   connect(&m_statReceivers, SIGNAL(editTextChanged(const QString&)), this,
           SLOT(statReceiverTextChangedSlot(const QString&)));

   m_currentStatReceiver = m_statReceivers.currentText();

   m_layout.addWidget(&m_removeStatReceiver, 3, 2, 1, 1, Qt::AlignLeft);
   m_removeStatReceiver.setEnabled(!confFileName.isEmpty());
   connect(&m_removeStatReceiver, SIGNAL(pressed()), this,
           SLOT(removeStatReceiverSlot()));

   //
   // Tweak the layout.
   //
   for (int i = 0; i < 4; ++i) {
      m_layout.setRowStretch(i, 0);
   }
   m_layout.setRowStretch(4, 1);
   for (int i = 0; i < 3; ++i) {
      m_layout.setColumnStretch(i, 1);
   }

   //
   // Create the File menu.
   //
   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

   connect(fileMenu->addAction(
               QIcon::fromTheme("document-open", QIcon{":/img/fileopen.png"}),
               tr("&Open configuration file...")),
           SIGNAL(triggered()), this, SLOT(readConfigSlot()));

   fileMenu->addSeparator();

   connect(fileMenu->addAction(
               QIcon::fromTheme("application-exit", QIcon{":/img/warning.png"}),
               tr("&Quit")),
           SIGNAL(triggered()), this, SLOT(close()));

   //
   // Create the Help menu.
   //
   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

   connect(helpMenu->addAction(QIcon{":/img/qt-logo.jpg"}, tr("About Qt")),
           SIGNAL(triggered()), qApp, SLOT(aboutQt()));

   connect(helpMenu->addAction(QIcon{":/img/caen.png"}, tr("About Caen DAQ")),
           SIGNAL(triggered()), this, SLOT(aboutCaenDAQSlot()));

   connect(helpMenu->addAction(QIcon{":/img/logo.png"}, tr("About CDA")),
           SIGNAL(triggered()), this, SLOT(aboutCDASlot()));
}

/**
 * This "Qt slot" opens a file selection window and then sets up all the
 * components of the window to use this new selected file.
 */
void MainWindow::readConfigSlot() {

   // Ask the user for a file name:
   QString fileName = QFileDialog::getOpenFileName(
       this, tr("Open setup file"), tr("default.cxml"),
       tr("CDA XML setup files (*.cxml);;"
          "All files (*)"));
   QApplication::setFont(gui::DefaultFont());

   // If the user selected a file, use it.
   if (!fileName.isEmpty()) {

      setWindowTitle(tr("CDA CAEN Digitizer DAQ - %1").arg(fileName));
      m_statistics.setEnabled(true);
      m_caenReader.setConfigFileName(fileName);
      m_caenReader.setEnabled(true);
      m_glomemWriter.setConfigFileName(fileName);
      m_glomemWriter.setEnabled(true);
      m_hbookWriter.setConfigFileName(fileName);
      m_hbookWriter.setEnabled(true);
      m_rootWriter.setConfigFileName(fileName);
      m_rootWriter.setEnabled(true);
      m_rawWriter.setConfigFileName(fileName);
      m_rawWriter.setEnabled(true);

      m_eventReceiversLabel.setEnabled(true);
      m_eventReceivers.setEnabled(true);
      m_removeEventReceiver.setEnabled(true);

      m_statReceiversLabel.setEnabled(true);
      m_statReceivers.setEnabled(true);
      m_removeStatReceiver.setEnabled(true);
   }
}

void MainWindow::aboutCaenDAQSlot() {

   QMessageBox::about(
       this, tr("CDA CAEN Digitizer DAQ"),
       tr("This application is a simplified interface for running "
          "a CDA CAEN Digitizer data acquisition session. While the CDA "
          "executables can be started on multiple separate computers, "
          "this application starts one instance of the command line CDA "
          "applications that can be used in CAEN data taking, on the "
          "local computer."));
   QApplication::setFont(gui::DefaultFont());
}

void MainWindow::aboutCDASlot() {

   aboutCDA(this);
}

void MainWindow::eventReceiverIndexChangedSlot(const QString& text) {

   // Remember the currently selected address:
   m_currentEventReceiver = text;
}

void MainWindow::eventReceiverTextChangedSlot(const QString& text) {

   // Remove the previous address from the event receiver list:
   m_caenReader.setWriterRunning(false, m_currentEventReceiver);
   // Add the current address as an event receiver:
   m_caenReader.setWriterRunning(true, text);
   // Update the current event receiver address:
   m_currentEventReceiver = text;
}

void MainWindow::removeEventReceiverSlot() {

   // Remove the event receiver from the CAEN reader runner:
   m_caenReader.setWriterRunning(false, m_eventReceivers.currentText());

   if (m_eventReceivers.count() > 1) {
      // Remove the current entry from the list:
      m_eventReceivers.removeItem(m_eventReceivers.currentIndex());
      m_currentEventReceiver = m_eventReceivers.currentText();
   } else {
      // Just clear the last entry:
      m_eventReceivers.setItemText(m_eventReceivers.currentIndex(), "");
      m_currentEventReceiver = "";
   }
}

void MainWindow::statReceiverIndexChangedSlot(const QString& text) {

   // Remember the currently selected address:
   m_currentStatReceiver = text;
}

void MainWindow::statReceiverTextChangedSlot(const QString& text) {

   // Remove the previous address from the event receiver list:
   m_caenReader.setStatServerAddress(false, m_currentStatReceiver);
   m_glomemWriter.setStatServerAddress(false, m_currentStatReceiver);
   m_hbookWriter.setStatServerAddress(false, m_currentStatReceiver);
   m_rootWriter.setStatServerAddress(false, m_currentStatReceiver);
   m_rawWriter.setStatServerAddress(false, m_currentStatReceiver);
   // Add the current address as an event receiver:
   m_caenReader.setStatServerAddress(true, text);
   m_glomemWriter.setStatServerAddress(true, text);
   m_hbookWriter.setStatServerAddress(true, text);
   m_rootWriter.setStatServerAddress(true, text);
   m_rawWriter.setStatServerAddress(true, text);
   // Update the current event receiver address:
   m_currentStatReceiver = text;
}

void MainWindow::removeStatReceiverSlot() {

   // Remove the current entry from the "runners":
   m_caenReader.setStatServerAddress(false, m_statReceivers.currentText());
   m_glomemWriter.setStatServerAddress(false, m_statReceivers.currentText());
   m_hbookWriter.setStatServerAddress(false, m_statReceivers.currentText());
   m_rootWriter.setStatServerAddress(false, m_statReceivers.currentText());
   m_rawWriter.setStatServerAddress(false, m_statReceivers.currentText());

   if (m_statReceivers.count() > 1) {
      // Remove the current entry from the list:
      m_statReceivers.removeItem(m_statReceivers.currentIndex());
      m_currentStatReceiver = m_statReceivers.currentText();
   } else {
      // Just clear the last entry:
      m_statReceivers.setItemText(m_statReceivers.currentIndex(), "");
      m_currentStatReceiver = "";
   }
}
