
// Qt include(s):
#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QFileDialog>
#include <QIcon>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSpinBox>
#include <QStatusBar>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomImplementation>

// CDA include(s):
#include "common/Address.h"
#include "common/DefaultFont.h"
#include "common/aboutCDA.h"
#include "device/Loader.h"
#include "event/Event.h"
#include "event/EventServer.h"

// Local include(s):
#include "../common/readConfig.h"
#include "QtMonitoringWindow.h"

/**
 * The constructor creates the layout of the window, and connects up the various
 * objects to interact with each other.
 */
QtMonitoringWindow::QtMonitoringWindow() : QMainWindow(), m_server(0) {

   resize(750, 500);
   setWindowTitle(tr("CDA Qt DAQ Monitoring"));
   setWindowIcon(QIcon(":/img/cda-qt-monitoring.png"));

   // Load all the plugins:
   dev::Loader::instance()->loadAll();
   setLoader(dev::Loader::instance());

   // Create the MDI Widget:
   m_view = new QMdiArea();
   setCentralWidget(m_view);

   //
   // Create the dock widgets and the menus:
   //
   createDockWidgets();
   createMenus();

   //
   // Set the behaviour of the dock widgets:
   //
   setDockNestingEnabled(true);
   setDockOptions(QMainWindow::AnimatedDocks);
}

/**
 * The destructor has to be pretty smart about deleting the objects,
 * because both the MDI area and the dev::Crate objects think that they
 * own the dev::QtHist objects. In the end we let the MDI area delete
 * them.
 */
QtMonitoringWindow::~QtMonitoringWindow() {

   if (m_server) {
      m_server->terminate();
      if (m_server->wait(1000)) {
         delete m_server;
      }
   }
   m_view->closeAllSubWindows();
   delete m_view;

   // At this point the dev::QtHist objects are doubly owned by the base class
   // and the Qt GUI objects. So now we need to make the base class let go of
   // them, so that Qt could delete them in peace.
   DeviceMap_t::iterator itr = m_devices.begin();
   DeviceMap_t::iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      itr->second.release();
   }
   m_devices.clear();

   delete m_showPortDock;

   delete m_portEdit;
   delete m_portDock;
}

void QtMonitoringWindow::readConfigSlot() {

   // Ask the user for a file name:
   const QString fileName = QFileDialog::getOpenFileName(
       this, tr("Open setup file"), tr("default.cxml"),
       tr("CDA XML setup files (*.cxml);;"
          "CDA binary setup files (*.cbin);;"
          "All files (*)"));
   QApplication::setFont(gui::DefaultFont());

   // If the cancel button has been pushed, don't continue:
   if (fileName.isEmpty())
      return;

   // Decide how to read the file:
   if (fileName.endsWith(".cbin")) {
      if (!readBinaryConfig(fileName)) {
         REPORT_ERROR(tr("Couldn't read configuration from %1").arg(fileName));
         return;
      }
   } else {
      if (!readXMLConfig(fileName)) {
         REPORT_ERROR(tr("Couldn't read configuration from %1").arg(fileName));
         return;
      }
   }

   // Remove the current windows:
   m_view->closeAllSubWindows();

   // Create a new QMdiSubWindow for each device:
   DeviceMap_t::const_iterator itr = m_devices.begin();
   DeviceMap_t::const_iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      QMdiSubWindow* window = m_view->addSubWindow(itr->second.get());
      window->setAttribute(Qt::WA_DeleteOnClose, false);
      window->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
      window->setWindowTitle(itr->second->windowTitle());
      connect(window, SIGNAL(aboutToActivate()), itr->second.get(),
              SLOT(update()));
      itr->second->show();
   }

   return;
}

void QtMonitoringWindow::aboutQtMonitoringSlot() {

   QMessageBox::about(
       this, tr("CDA Qt Monitoring"),
       tr("This application can be used to monitor a data taking "
          "session without the need for any external libraries.\n\n"
          "Normally the data taking is monitored using PAW/CERNLIB, "
          "with the help of the cda-glomem-writer application. "
          "This application is most useful when CERNLIB is not "
          "avalable."));

   // Make sure Qt remembers what is its default font.
   // (There seems to be a bug in Qt...)
   QApplication::setFont(gui::DefaultFont());

   return;
}

void QtMonitoringWindow::aboutCDASlot() {

   aboutCDA(this);
   return;
}

void QtMonitoringWindow::portChangedSlot() {

   const int port = m_portEdit->value();
   static int previous_port = 0;
   if (port == previous_port) {
      return;
   } else {
      previous_port = port;
   }
   // Terminate the previous thread, or start a new one:
   if (m_server) {
      m_server->terminate();
      if (!m_server->wait(1000)) {
         QMessageBox::critical(this, tr("Event server thread error"),
                               tr("Couldn't cleanly terminate the event "
                                  "reading thread. This is strange."));
         delete m_server;
         m_server = new ev::EventServer(10000, this);
         connect(m_server, SIGNAL(eventAvailable()), this,
                 SLOT(processEvents()));
      }
   } else {
      m_server = new ev::EventServer(10000, this);
      connect(m_server, SIGNAL(eventAvailable()), this, SLOT(processEvents()));
   }
   // Start the server once more:
   m_server->listen(Address("0.0.0.0", port));

   statusBar()->showMessage(tr("Event server is running on port %1").arg(port));

   return;
}

void QtMonitoringWindow::processEvents() {

   // Read out all the events that are in the buffer at the moment:
   for (size_t i = 0; i < m_server->bufferSize(); ++i) {

      // Read out one event from the buffer:
      ev::Event event;
      (*m_server) >> event;

      // Loop over the fragments coming from the different modules that
      // are used in data acquisition:
      for (auto& fragment : event.getFragments()) {

         // Find the device that is expecting this event fragment:
         DeviceMap_t::iterator device = m_devices.find(fragment->getModuleID());
         if (device == m_devices.end()) {
            REPORT_ERROR(tr("Failed to assign fragment with "
                            "module ID: %1")
                             .arg(fragment->getModuleID()));
            return;
         }

         // Give this fragment to the device that we just found:
         if (!device->second->displayEvent(*fragment)) {
            REPORT_ERROR(tr("There was a problem displaying the data "
                            "from device with ID: %1")
                             .arg(fragment->getModuleID()));
            return;
         }
      }
   }

   return;
}

bool QtMonitoringWindow::readXMLConfig(const QString& filename) {

   // Read the configuration.
   if (apps::readConfig(filename, *this).isFailure()) {
      REPORT_ERROR(tr("Couldn't read configuration from %1").arg(filename));
      return false;
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split("/");
   setWindowTitle(tr("CDA Qt DAQ Monitoring - %1").arg(filePath.back()));

   return true;
}

bool QtMonitoringWindow::readBinaryConfig(const QString& filename) {

   //
   // Open the input file:
   //
   QFile input_file(filename);
   if (!input_file.open(QFile::ReadOnly)) {
      REPORT_ERROR(tr("The specified configuration file (\"%1\")"
                      " could not be opened!")
                       .arg(filename));
      QMessageBox::critical(this, tr("File reading error"),
                            tr("The specified configuration file "
                               "(\"%1\") could not be opened!")
                                .arg(filename));
      return false;
   } else {
      REPORT_VERBOSE(tr("Opened file: %1").arg(filename));
   }

   //
   // Read the configuration from this file:
   //
   if (!readConfig(input_file)) {
      REPORT_ERROR(
          tr("Some error happened while reading the "
             "binary configuration"));
      QMessageBox::critical(
          this, tr("Configuration reading error"),
          tr("The configuration could not be read. See "
             "the application messages for more information"));
      return false;
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split("/");
   setWindowTitle(tr("CDA Qt DAQ Monitoring - %1").arg(filePath.back()));

   return true;
}

/**
 * This is where the menus are set up. Notice that the function assumes that
 * the dock widgets have already been set up, so this function has to be
 * called after createDockWidgets() has already run.
 */
void QtMonitoringWindow::createMenus() {

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                    Create a "File" menu                     //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

   QAction* openAction = fileMenu->addAction(
       QIcon::fromTheme("document-open", QIcon(":/img/fileopen.png")),
       tr("&Open configuration file..."));
   connect(openAction, SIGNAL(triggered()), this, SLOT(readConfigSlot()));

   QAction* quitAction = fileMenu->addAction(
       QIcon::fromTheme("application-exit", QIcon(":/img/warning.png")),
       tr("&Quit"));
   connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                Create a "Preferences" menu                  //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* prefMenu = menuBar()->addMenu(tr("&Preferences"));

   m_showPortDock = prefMenu->addAction(tr("Show port setting"));
   m_showPortDock->setCheckable(true);
   connect(m_showPortDock, SIGNAL(toggled(bool)), m_portDock,
           SLOT(setVisible(bool)));
   connect(m_portDock, SIGNAL(visibilityChanged(bool)), m_showPortDock,
           SLOT(setChecked(bool)));

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                    Create a "Help" menu                     //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

   QAction* aboutQtAction =
       helpMenu->addAction(QIcon(":/img/qt-logo.jpg"), tr("About &Qt"));
   connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

   QAction* aboutQtMonitoringAction = helpMenu->addAction(
       QIcon(":/img/cda-qt-monitoring.png"), tr("&About Qt DAQ Monitoring"));
   connect(aboutQtMonitoringAction, SIGNAL(triggered()), this,
           SLOT(aboutQtMonitoringSlot()));

   QAction* aboutCDAAction =
       helpMenu->addAction(QIcon(":/img/logo.png"), tr("About &CDA"));
   connect(aboutCDAAction, SIGNAL(triggered()), this, SLOT(aboutCDASlot()));

   return;
}

void QtMonitoringWindow::createDockWidgets() {

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //              Create the port editing widgets                //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_portEdit = new QSpinBox();
   m_portEdit->setMinimum(100);
   m_portEdit->setMaximum(80000);
   m_portEdit->setValue(35000);
   connect(m_portEdit, SIGNAL(editingFinished()), this,
           SLOT(portChangedSlot()));

   m_portDock = new QDockWidget(tr("Event server port"), this);
   m_portDock->setAllowedAreas(Qt::TopDockWidgetArea |
                               Qt::BottomDockWidgetArea);
   m_portDock->setWidget(m_portEdit);

   addDockWidget(Qt::TopDockWidgetArea, m_portDock);

   return;
}
