// $Id$

// Qt include(s):
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QFileDialog>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStackedWidget>
#include <QWidget>
#include <QtCore/QFile>
#include <QtCore/QFlags>
#include <QtCore/QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomImplementation>

// CDA include(s):
#include "common/DefaultFont.h"
#include "common/aboutCDA.h"

// Local include(s):
#include "ConfigEditorWindow.h"

/**
 * The constructor creates and arranges all the graphical widgets.
 */
ConfigEditorWindow::ConfigEditorWindow()
    : QMainWindow(), m_currFileName(""), m_logger("ConfigEditorWindow") {

   // Make sure Qt remembers what is its default font:
   QApplication::setFont(gui::DefaultFont());

   //
   // Set the fixed size of the window:
   //
#ifdef Q_OS_DARWIN
   resize(540, 660);
   setMinimumSize(540, 660);
   setMaximumSize(540, 660);
#else
   resize(540, 690);
   setMinimumSize(540, 690);
   setMaximumSize(540, 690);
#endif

   //
   // Set the main window properties:
   //
   setWindowTitle(tr("CDA configuration editor - Untitled.cxml"));
   setWindowIcon(QIcon(":/img/cda-config-editor.png"));

   //
   // Create a central widget for the window:
   //
   m_centralWidget.reset(new QWidget(this));
   m_centralWidget->resize(540, 660);
   m_centralWidget->setMinimumSize(540, 660);
   m_centralWidget->setMaximumSize(540, 660);
   setCentralWidget(m_centralWidget.get());

   //
   // Create a widget stack for the configuration objects:
   //
   m_editStack.reset(new QStackedWidget(m_centralWidget.get()));
   m_editStack->setGeometry(QRect(10, 10, 520, 640));

   //
   // Create the CAMAC crate editor:
   //
   m_camacEdit.reset(new dev::CamacEditor(m_editStack.get()));
   m_camacEdit->setGeometry(QRect(0, 0, 520, 640));
   m_editStack->addWidget(m_camacEdit.get());

   //
   // Create the CAEN digitizer device editor:
   //
   m_caenDigitizerEdit.reset(new dev::CaenDigitizerEditor(m_editStack.get()));
   m_caenDigitizerEdit->setGeometry(QRect(0, 0, 520, 640));
   m_editStack->addWidget(m_caenDigitizerEdit.get());

   //
   // Create the CAEN VME device editor:
   //
   m_caenVmeEdit.reset(new dev::CaenVmeEditor(m_editStack.get()));
   m_caenVmeEdit->setGeometry(QRect(0, 0, 520, 640));
   m_editStack->addWidget(m_caenVmeEdit.get());

   //
   // Create the application menus:
   //
   createMenus();

   // Show the CAMAC editor by default:
   showCamacConfigSlot();
}

void ConfigEditorWindow::newConfigSlot() {

   m_camacEdit->clear();
   m_caenDigitizerEdit->clear();
   m_caenVmeEdit->clear();
   showCamacConfigSlot();
   setWindowTitle(tr("CDA configuration editor - Untitled.cxml"));

   return;
}

void ConfigEditorWindow::readConfigSlot() {

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

   // Save the file name for later:
   m_currFileName = fileName;

   // Decide how to read the file:
   if (m_currFileName.endsWith(".cbin")) {
      readBinaryConfig(m_currFileName);
   } else {
      readXMLConfig(m_currFileName);
   }

   return;
}

void ConfigEditorWindow::writeConfigSlot() {

   if (m_currFileName.isEmpty()) {
      writeConfigAsSlot();
   }

   // Decide how to write the file:
   if (m_currFileName.endsWith(".cbin")) {
      writeBinaryConfig(m_currFileName);
   } else {
      writeXMLConfig(m_currFileName);
   }

   return;
}

void ConfigEditorWindow::writeConfigAsSlot() {

   const QString fileName = QFileDialog::getSaveFileName(
       this, tr("Save setup file"), tr("default.cxml"),
       tr("CDA XML setup files (*.cxml);;"
          "CDA binary setup files (*.cbin)"));
   QApplication::setFont(gui::DefaultFont());

   // If the cancel button has been pushed, don't continue:
   if (fileName.isEmpty())
      return;

   // Save the file name for later:
   m_currFileName = fileName;

   // Decide how to write the file:
   if (m_currFileName.endsWith(".cbin")) {
      writeBinaryConfig(m_currFileName);
   } else {
      writeXMLConfig(m_currFileName);
   }

   return;
}

void ConfigEditorWindow::showCamacConfigSlot() {

   // Bring the Camac configuration to the front if it's not there
   // already:
   if (m_editStack->currentWidget() != m_camacEdit.get()) {
      m_editStack->setCurrentWidget(m_camacEdit.get());
   }
   // Make sure the menu is in sync with what is shown:
   if (!m_camacConfigAction->isChecked()) {
      m_camacConfigAction->setChecked(true);
   }

   return;
}

void ConfigEditorWindow::showCaenDigitizerConfigSlot() {

   // Bring the Caen configuration to the front if it's not there
   // already:
   if (m_editStack->currentWidget() != m_caenDigitizerEdit.get()) {
      m_editStack->setCurrentWidget(m_caenDigitizerEdit.get());
   }
   // Make sure the menu is in sync with what is shown:
   if (!m_caenDigitizerConfigAction->isChecked()) {
      m_caenDigitizerConfigAction->setChecked(true);
   }

   return;
}

void ConfigEditorWindow::showCaenVmeConfigSlot() {

   // Bring the Caen configuration to the front if it's not there
   // already:
   if (m_editStack->currentWidget() != m_caenVmeEdit.get()) {
      m_editStack->setCurrentWidget(m_caenVmeEdit.get());
   }
   // Make sure the menu is in sync with what is shown:
   if (!m_caenVmeConfigAction->isChecked()) {
      m_caenVmeConfigAction->setChecked(true);
   }

   return;
}

void ConfigEditorWindow::aboutCDASlot() {

   aboutCDA(this);
   return;
}

void ConfigEditorWindow::aboutConfigEditorSlot() {

   QMessageBox::about(this, tr("CDA Configuration Editor"),
                      tr("This application can be used to construct "
                         "configuration files for the CDA applications. "
                         "The files can be read and saved in both binary "
                         "and XML format."));

   // Make sure Qt remembers what is its default font.
   // (There seems to be a bug in Qt...)
   QApplication::setFont(gui::DefaultFont());

   return;
}

void ConfigEditorWindow::createMenus() {

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the File menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

   QAction* newAction = fileMenu->addAction(
       QIcon::fromTheme("document-new", QIcon(":/img/filenew.png")),
       tr("&Create new configuration..."));
   connect(newAction, SIGNAL(triggered()), this, SLOT(newConfigSlot()));

   QAction* openAction = fileMenu->addAction(
       QIcon::fromTheme("document-open", QIcon(":/img/fileopen.png")),
       tr("&Open configuration file..."));
   connect(openAction, SIGNAL(triggered()), this, SLOT(readConfigSlot()));

   QAction* saveAction = fileMenu->addAction(
       QIcon::fromTheme("document-save", QIcon(":/img/filesave.png")),
       tr("&Save configuration file"));
   connect(saveAction, SIGNAL(triggered()), this, SLOT(writeConfigSlot()));

   QAction* saveAsAction = fileMenu->addAction(
       QIcon::fromTheme("document-save-as", QIcon(":/img/filesaveas.png")),
       tr("Save configuration as..."));
   connect(saveAsAction, SIGNAL(triggered()), this, SLOT(writeConfigAsSlot()));

   fileMenu->addSeparator();

   QAction* quitAction = fileMenu->addAction(
       QIcon::fromTheme("application-exit", QIcon(":/img/warning.png")),
       tr("&Quit"));
   connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                Create the Config menu                   //
   //                                                         //
   /////////////////////////////////////////////////////////////

   QMenu* configMenu = menuBar()->addMenu(tr("&Config"));

   m_camacConfigAction.reset(new QAction(tr("Show CAMAC Config"), this));
   m_camacConfigAction->setCheckable(true);
   connect(m_camacConfigAction.get(), SIGNAL(triggered()), this,
           SLOT(showCamacConfigSlot()));
   m_caenDigitizerConfigAction.reset(
       new QAction(tr("Show CAEN Digitizer Config"), this));
   m_caenDigitizerConfigAction->setCheckable(true);
   connect(m_caenDigitizerConfigAction.get(), SIGNAL(triggered()), this,
           SLOT(showCaenDigitizerConfigSlot()));
   m_caenVmeConfigAction.reset(new QAction(tr("Show CAEN VME Config"), this));
   m_caenVmeConfigAction->setCheckable(true);
   connect(m_caenVmeConfigAction.get(), SIGNAL(triggered()), this,
           SLOT(showCaenVmeConfigSlot()));

   QActionGroup* configGroup = new QActionGroup(this);
   configGroup->setExclusive(true);
   configGroup->addAction(m_camacConfigAction.get());
   configGroup->addAction(m_caenDigitizerConfigAction.get());
   configGroup->addAction(m_caenVmeConfigAction.get());
   m_camacConfigAction->setChecked(true);

   configMenu->addAction(m_camacConfigAction.get());
   configMenu->addAction(m_caenDigitizerConfigAction.get());
   configMenu->addAction(m_caenVmeConfigAction.get());

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the Help menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu(tr("&Help"));

   QAction* aboutQtAction =
       helpMenu->addAction(QIcon(":/img/qt-logo.jpg"), tr("About Qt"));
   connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

   QAction* aboutConfigEditorAc = helpMenu->addAction(
       QIcon(":/img/cda-config-editor.png"), tr("About Config Editor"));
   connect(aboutConfigEditorAc, SIGNAL(triggered()), this,
           SLOT(aboutConfigEditorSlot()));

   QAction* aboutCDAAction =
       helpMenu->addAction(QIcon(":/img/logo.png"), tr("About CDA"));
   connect(aboutCDAAction, SIGNAL(triggered()), this, SLOT(aboutCDASlot()));

   return;
}

void ConfigEditorWindow::readXMLConfig(const QString& filename) {

   //
   // Open the configuration file:
   //
   QFile config_file(filename);
   if (!config_file.open(QFile::ReadOnly | QFile::Text)) {
      REPORT_ERROR(tr("The specified configuration file (\"%1\")"
                      " could not be opened!")
                       .arg(filename));
      QMessageBox::critical(this, tr("File not opened"),
                            tr("The specified configuration file (\"%1\")"
                               " could not be opened!")
                                .arg(filename));
      return;
   } else {
      REPORT_VERBOSE(tr("Opened file: %1").arg(filename));
   }

   //
   // Read the file's contents into XML format:
   //
   QDomDocument doc;
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
   const auto result = doc.setContent(&config_file);
   if (!result) {
      REPORT_ERROR(tr("Error in parsing \"%1\"\n"
                      "  Error message: %2\n"
                      "  Error line   : %3\n"
                      "  Error column : %4")
                       .arg(filename)
                       .arg(result.errorMessage)
                       .arg(result.errorLine)
                       .arg(result.errorColumn));
      QMessageBox::critical(this, tr("XML error"),
                            tr("There is some problem with the format of the "
                               "input file.\n\n "
                               "Error message: %1\n"
                               "Error line: %2\n"
                               "Error column: %3")
                                .arg(result.errorMessage)
                                .arg(result.errorLine)
                                .arg(result.errorColumn));
#else
   QString errorMsg;
   int errorLine, errorColumn;
   if (!doc.setContent(&config_file, false, &errorMsg, &errorLine,
                       &errorColumn)) {
      REPORT_ERROR(tr("Error in parsing \"%1\"\n"
                      "  Error message: %2\n"
                      "  Error line   : %3\n"
                      "  Error column : %4")
                       .arg(filename)
                       .arg(errorMsg)
                       .arg(errorLine)
                       .arg(errorColumn));
      QMessageBox::critical(this, tr("XML error"),
                            tr("There is some problem with the format of the "
                               "input file.\n\n "
                               "Error message: %1\n"
                               "Error line: %2\n"
                               "Error column: %3")
                                .arg(errorMsg)
                                .arg(errorLine)
                                .arg(errorColumn));
#endif
      return;
   } else {
      m_logger << msg::DEBUG << tr("Successfully parsed: %1").arg(filename)
               << msg::endmsg;
   }

   //
   // Let the appropriate object read the configuration:
   //
   const QDomElement work = doc.documentElement();
   if (m_camacEdit->canRead(work)) {
      if (!m_camacEdit->readConfig(work)) {
         REPORT_ERROR(tr("Failed to read CAMAC configuration file!"));
         QMessageBox::critical(this, tr("CAMAC configuration problem"),
                               tr("Failed to read CAMAC configuration "
                                  "file!"));
         return;
      }
      showCamacConfigSlot();
   } else if (m_caenDigitizerEdit->canRead(work)) {
      if (!m_caenDigitizerEdit->readConfig(work)) {
         REPORT_ERROR(
             tr("Failed to read CAEN digitizer configuration "
                "file!"));
         QMessageBox::critical(this,
                               tr("CAEN digitizer configuration "
                                  "problem"),
                               tr("Failed to read CAEN digitizer "
                                  "configuration file!"));
         return;
      }
      showCaenDigitizerConfigSlot();
   } else if (m_caenVmeEdit->canRead(work)) {
      if (!m_caenVmeEdit->readConfig(work)) {
         REPORT_ERROR(tr("Failed to read CAEN VME configuration file!"));
         QMessageBox::critical(this, tr("CAEN VME configuration problem"),
                               tr("Failed to read CAEN VME configuration "
                                  "file!"));
         return;
      }
      showCaenVmeConfigSlot();
   } else {
      REPORT_ERROR(
          tr("Couldn't find an editor able to handle the "
             "specified configuration file"));
      QMessageBox::critical(this, tr("Configuration unknown"),
                            tr("Couldn't find an editor able to handle the "
                               "specified configuration file"));
      return;
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split("/");
   setWindowTitle(tr("CDA configuration editor - %1").arg(filePath.back()));

   return;
}

void ConfigEditorWindow::readBinaryConfig(const QString& filename) {

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
      return;
   } else {
      REPORT_VERBOSE(tr("Opened file: %1").arg(filename));
   }

   //
   // Read the configuration from this file:
   //
   if (input_file.seek(0) && m_camacEdit->canRead(input_file)) {
      input_file.seek(0);
      if (!m_camacEdit->readConfig(input_file)) {
         REPORT_ERROR(
             tr("Some error happened while reading the "
                "binary configuration"));
         QMessageBox::critical(this, tr("Configuration reading error"),
                               tr("The configuration could not be read. See "
                                  "the application messages for more "
                                  "information"));
         return;
      }
      showCamacConfigSlot();
   } else if (input_file.seek(0) && m_caenDigitizerEdit->canRead(input_file)) {
      input_file.seek(0);
      if (!m_caenDigitizerEdit->readConfig(input_file)) {
         REPORT_ERROR(
             tr("Some error happened while reading the "
                "binary configuration"));
         QMessageBox::critical(this, tr("Configuration reading error"),
                               tr("The configuration could not be read. See "
                                  "the application messages for more "
                                  "information"));
         return;
      }
      showCaenDigitizerConfigSlot();
   } else if (input_file.seek(0) && m_caenVmeEdit->canRead(input_file)) {
      input_file.seek(0);
      if (!m_caenVmeEdit->readConfig(input_file)) {
         REPORT_ERROR(
             tr("Some error happened while reading the "
                "binary configuration"));
         QMessageBox::critical(this, tr("Configuration reading error"),
                               tr("The configuration could not be read. See "
                                  "the application messages for more "
                                  "information"));
         return;
      }
      showCaenVmeConfigSlot();
   } else {
      REPORT_ERROR(
          tr("Couldn't find an editor able to handle the "
             "specified configuration file"));
      QMessageBox::critical(this, tr("Configuration unknown"),
                            tr("Couldn't find an editor able to handle the "
                               "specified configuration file"));
      return;
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split("/");
   setWindowTitle(tr("CDA configuration editor - %1").arg(filePath.back()));

   return;
}

void ConfigEditorWindow::writeXMLConfig(const QString& filename) {

   //
   // Create a DOM document:
   //
   QDomImplementation imp;
   QDomDocument doc = imp.createDocument(
       "", "Setup", imp.createDocumentType("CDA_SETUP", "", ""));

   //
   // Write the configuration to this document:
   //
   QDomElement elem = doc.documentElement();
   if (m_editStack->currentWidget() == m_camacEdit.get()) {
      if (!m_camacEdit->writeConfig(elem)) {
         REPORT_ERROR(
             tr("Some error happened while creating the "
                "XML configuration"));
         QMessageBox::critical(this, tr("Configuration writing error"),
                               tr("The configuration could not be written."
                                  " See the application messages for more "
                                  "information."));
         return;
      }
   } else if (m_editStack->currentWidget() == m_caenDigitizerEdit.get()) {
      if (!m_caenDigitizerEdit->writeConfig(elem)) {
         REPORT_ERROR(
             tr("Some error happened while creating the "
                "XML configuration"));
         QMessageBox::critical(this, tr("Configuration writing error"),
                               tr("The configuration could not be written."
                                  "See the application messages for more "
                                  "information."));
         return;
      }
   } else if (m_editStack->currentWidget() == m_caenVmeEdit.get()) {
      if (!m_caenVmeEdit->writeConfig(elem)) {
         REPORT_ERROR(
             tr("Some error happened while creating the "
                "XML configuration"));
         QMessageBox::critical(this, tr("Configuration writing error"),
                               tr("The configuration could not be written."
                                  "See the application messages for more "
                                  "information."));
         return;
      }
   }

   //
   // Write the document to a file:
   //
   QFile output_file(filename);
   if (!output_file.open(QFile::WriteOnly | QFile::Truncate)) {
      REPORT_ERROR(tr("%1 couldn't be opened "
                      "for writing")
                       .arg(filename));
      QMessageBox::critical(this, tr("File writing error"),
                            tr("The selected file could not be opened for "
                               "writing. Check permissions!"));
      return;
   } else {
      m_logger << msg::DEBUG << tr("%1 opened for writing").arg(filename)
               << msg::endmsg;
   }

   QTextStream stream(&output_file);
   doc.save(stream, 3);

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split("/");
   setWindowTitle(tr("CDA configuration editor - %1").arg(filePath.back()));

   return;
}

void ConfigEditorWindow::writeBinaryConfig(const QString& filename) {

   //
   // Open the output file:
   //
   QFile output_file(filename);
   if (!output_file.open(QFile::WriteOnly | QFile::Truncate)) {
      REPORT_ERROR(tr("%1 couldn't be opened "
                      "for writing")
                       .arg(filename));
      QMessageBox::critical(this, tr("File writing error"),
                            tr("The selected file could not be opened for "
                               "writing. Check permissions!"));
      return;
   } else {
      m_logger << msg::DEBUG << tr("%1 opened for writing").arg(filename)
               << msg::endmsg;
   }

   //
   // Write the configuration to this file:
   //
   if (m_editStack->currentWidget() == m_camacEdit.get()) {
      if (!m_camacEdit->writeConfig(output_file)) {
         REPORT_ERROR(
             tr("Some error happened while creating the "
                "binary configuration"));
         QMessageBox::critical(this, tr("Configuration writing error"),
                               tr("The configuration could not be written."
                                  " See the application messages for more "
                                  "information."));
         return;
      }
   } else if (m_editStack->currentWidget() == m_caenDigitizerEdit.get()) {
      if (!m_caenDigitizerEdit->writeConfig(output_file)) {
         REPORT_ERROR(
             tr("Some error happened while creating the "
                "binary configuration"));
         QMessageBox::critical(this, tr("Configuration writing error"),
                               tr("The configuration could not be written."
                                  " See the application messages for more "
                                  "information."));
         return;
      }
   } else if (m_editStack->currentWidget() == m_caenVmeEdit.get()) {
      if (!m_caenVmeEdit->writeConfig(output_file)) {
         REPORT_ERROR(
             tr("Some error happened while creating the "
                "binary configuration"));
         QMessageBox::critical(this, tr("Configuration writing error"),
                               tr("The configuration could not be written."
                                  " See the application messages for more "
                                  "information."));
         return;
      }
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split("/");
   setWindowTitle(tr("CDA configuration editor - ") + filePath.back());

   return;
}
