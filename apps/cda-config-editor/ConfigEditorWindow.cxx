// $Id$

// Qt include(s):
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QWidget>
#include <QtGui/QListWidget>
#include <QtGui/QListWidgetItem>
#include <QtGui/QIcon>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtXml/QDomImplementation>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdagui/device/Editor.h"
#   include "cdagui/common/aboutCDA.h"
#else
#   include "device/Editor.h"
#   include "common/aboutCDA.h"
#endif

// Local include(s):
#include "ConfigEditorWindow.h"

/**
 * The constructor creates and arranges all the graphical widgets.
 */
ConfigEditorWindow::ConfigEditorWindow()
   : QMainWindow(), m_currFileName( "" ), m_logger( "ConfigEditorWindow" ) {

   //
   // Set the fixed size of the window:
   //
#ifdef Q_OS_DARWIN
   resize( 645, 660 );
   setMinimumSize( 645, 660 );
   setMaximumSize( 645, 660 );
#else
   resize( 645, 690 );
   setMinimumSize( 645, 690 );
   setMaximumSize( 645, 690 );
#endif

   //
   // Set the main window properties:
   //
   setWindowTitle( tr( "CDA configuration editor - Untitled.cxml" ) );
   setWindowIcon( QIcon( ":/img/logo.png" ) );

   //
   // Create a central widget for the window:
   //
   m_centralWidget = new QWidget( this );
   m_centralWidget->resize( 645, 660 );
   m_centralWidget->setMinimumSize( 645, 660 );
   m_centralWidget->setMaximumSize( 645, 660 );
   setCentralWidget( m_centralWidget );

   //
   // Create the icon view for selecting the configuration "aspect":
   //
   m_setupSelect = new QListWidget( m_centralWidget );
   m_setupSelect->setViewMode( QListView::IconMode );
   m_setupSelect->setGeometry( QRect( 10, 10, 95, 640 ) );

   //
   // Create the icon for the CAMAC crate configuration:
   //
   m_deviceSetup = new QListWidgetItem( QIcon( ":/img/crate-settings.png" ),
                                        tr( "CAMAC setup" ), m_setupSelect );
   m_deviceSetup->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
   m_deviceSetup->setTextAlignment( Qt::AlignHCenter | Qt::AlignBottom );

   //
   // Create the CAMAC crate editor:
   //
   m_devEdit = new dev::Editor( m_centralWidget );
   m_devEdit->setGeometry( QRect( 115, 10, 520, 640 ) );

   //
   // Create the application menus:
   //
   createMenus();

}

void ConfigEditorWindow::setupSelectActivated( QListWidgetItem* ) {
/*
	if (m_deviceSetup==item)
	{
			m_devEdit->show();
			m_netEdit->hide();
	} else if  (m_networkSetup==item)
	{
			m_devEdit->hide();
			m_netEdit->show();
	}
	*/
}

void ConfigEditorWindow::newConfigSlot() {

   m_devEdit->clear();
   setWindowTitle( tr( "CDA configuration editor - Untitled.cxml" ) );
   return;

}

void ConfigEditorWindow::readConfigSlot() {

   // Ask the user for a file name:
   QString fileName = QFileDialog::getOpenFileName( this, tr( "Open setup file" ),
                                                    tr( "default.cxml" ),
                                                    tr( "CDA XML setup files (*.cxml);;"
                                                        "CDA binary setup files (*.cbin);;"
                                                        "All files (*)" ) );

   // If the cancel button has been pushed, don't continue:
   if( fileName.isEmpty() ) return;

   // Save the file name for later:
   m_currFileName = fileName;

   // Decide how to read the file:
   if( m_currFileName.endsWith( ".cbin" ) ) {
      readBinaryConfig( m_currFileName );
   } else {
      readXMLConfig( m_currFileName );
   }

   return;

}

void ConfigEditorWindow::writeConfigSlot() {

   if( m_currFileName.isEmpty() ) {
      writeConfigAsSlot();
   }

   // Decide how to write the file:
   if( m_currFileName.endsWith( ".cbin" ) ) {
      writeBinaryConfig( m_currFileName );
   } else {
      writeXMLConfig( m_currFileName );
   }

   return;

}

void ConfigEditorWindow::writeConfigAsSlot() {

   QString fileName = QFileDialog::getSaveFileName( this, tr( "Save setup file" ),
                                                    tr( "default.cxml" ),
                                                    tr( "CDA XML setup files (*.cxml);;"
                                                    "CDA binary setup files (*.cbin)" ) );

   // If the cancel button has been pushed, don't continue:
   if( fileName.isEmpty() ) return;

   // Save the file name for later:
   m_currFileName = fileName;

   // Decide how to write the file:
   if( m_currFileName.endsWith( ".cbin" ) ) {
      writeBinaryConfig( m_currFileName );
   } else {
      writeXMLConfig( m_currFileName );
   }

   return;

}

void ConfigEditorWindow::aboutQtSlot() {

   QMessageBox::aboutQt( this, tr( "CDA Configuration Editor - built on Qt" ) );
   return;

}

void ConfigEditorWindow::aboutCDASlot() {

   aboutCDA( this );
   return;

}

void ConfigEditorWindow::aboutConfigEditorSlot() {

   QMessageBox::about( this, tr( "CDA Configuration Editor" ),
                       tr( "This application can be used to construct "
                           "configuration files for the CDA applications. "
                           "The files can be read and saved in both binary "
                           "and XML format." ) );
   return;

}

void ConfigEditorWindow::createMenus() {

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the File menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu( tr( "&File" ) );

   QAction* newAction = fileMenu->addAction( QIcon( ":/img/filenew.png" ),
                                             tr( "&Create new configuration..." ) );
   connect( newAction, SIGNAL( triggered() ),
            this, SLOT( newConfigSlot() ) );

   QAction* openAction = fileMenu->addAction( QIcon( ":/img/fileopen.png" ),
                                              tr( "&Open configuration file..." ) );
   connect( openAction, SIGNAL( triggered() ),
            this, SLOT( readConfigSlot() ) );

   QAction* saveAction = fileMenu->addAction( QIcon( ":/img/filesave.png" ),
                                              tr( "&Save configuration file" ) );
   connect( saveAction, SIGNAL( triggered() ),
            this, SLOT( writeConfigSlot() ) );

   QAction* saveAsAction = fileMenu->addAction( QIcon( ":/img/filesaveas.png" ),
                                                tr( "Save configuration as..." ) );
   connect( saveAsAction, SIGNAL( triggered() ),
            this, SLOT( writeConfigAsSlot() ) );

   fileMenu->addSeparator();

   QAction* quitAction = fileMenu->addAction( QIcon( ":/img/warning.png" ),
                                              tr( "&Quit" ) );
   connect( quitAction, SIGNAL( triggered() ),
            this, SLOT( close() ) );

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the Help menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu( tr( "&Help" ) );

   QAction* aboutQtAction = helpMenu->addAction( tr( "About Qt" ) );
   connect( aboutQtAction, SIGNAL( triggered() ),
            this, SLOT( aboutQtSlot() ) );

   QAction* aboutConfigEditorAc = helpMenu->addAction( tr( "About Config Editor" ) );
   connect( aboutConfigEditorAc, SIGNAL( triggered() ),
            this, SLOT( aboutConfigEditorSlot() ) );

   QAction* aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                                  tr( "About CDA" ) );
   connect( aboutCDAAction, SIGNAL( triggered() ),
            this, SLOT( aboutCDASlot() ) );

   return;

}

inline void  ConfigEditorWindow::readErrorDlg()
{
	QMessageBox::critical( this, tr( "Configuration reading error" ),
                             tr( "The configuration could not be read. See "
                                 "the application messages for more information" ) );
}

void ConfigEditorWindow::readXMLConfig( const QString& filename ) {

   //
   // Open the configuration file:
   //
   QFile config_file( filename );
   if( ! config_file.open( QFile::ReadOnly | QFile::Text ) ) {
      m_logger << msg::ERROR << tr( "The specified configuration file (\"" )
               << filename << "\")" << std::endl
               << "could not be opened!" << msg::endmsg;
      QMessageBox::critical( this, tr( "File not opened" ),
                             tr( "The specified XML file (\"" ) + filename +
                             tr( "\") could not be opened!" ) );
      return;
   } else {
      m_logger << msg::VERBOSE << "Opened file: " << filename << msg::endmsg;
   }

   //
   // Read the file's contents into XML format:
   //
   QDomDocument doc;
   QString errorMsg;
   int errorLine, errorColumn;
   if( ! doc.setContent( &config_file, false, &errorMsg, &errorLine,
                         &errorColumn ) ) {
      m_logger << msg::ERROR << "Error in parsing \"" << filename
               << "\"" << std::endl
               << "  Error message: " << errorMsg << std::endl
               << "  Error line   : " << errorLine << std::endl
               << "  Error column : " << errorColumn << msg::endmsg;
      QMessageBox::critical( this, tr( "XML error" ),
                             tr( "There is some problem with the format of the "
                                 "input file.\n\n " ) +
                             tr( "Error message: " ) + errorMsg + "\n " +
                             tr( "Error line: " ) + QString::number( errorLine ) + "\n " +
                             tr( "Error column: " ) + QString::number( errorColumn ) );
      return;
   } else {
      m_logger << msg::DEBUG << "Successfully parsed: "
               << filename << msg::endmsg;
   }

   //
   // Let the appropriate object read the configuration:
   //
   QDomElement work = doc.documentElement();

   if( ! m_devEdit->readConfig( work ) ) { 
      m_logger << msg::ERROR << "Failed to read configuration file!"
               << msg::endmsg;
      return;
   } 

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split( "/" );
   setWindowTitle( tr( "CDA configuration editor - " ) + filePath.back() );

   return;

}

void ConfigEditorWindow::readBinaryConfig( const QString& filename ) {

   //
   // Open the input file:
   //
   QFile input_file( filename );
   if( ! input_file.open( QFile::ReadOnly ) ) {
      m_logger << msg::ERROR << filename << " couldn't be opened "
               << "for reading" << msg::endmsg;
      QMessageBox::critical( this, tr( "File reading error" ),
                             tr( "The selected file could not be opened for "
                                 "reading. Check the file name!" ) );
      return;
   } else {
      m_logger << msg::DEBUG << filename << " opened for reading"
               << msg::endmsg;
   }

   //
   // Read the configuration from this file:
   //
   if( ! m_devEdit->readConfig( &input_file ) ) {
      m_logger << msg::ERROR << "Some error happened while reading the "
               << "binary configuration" << msg::endmsg;
      QMessageBox::critical( this, tr( "Configuration reading error" ),
                             tr( "The configuration could not be read. See "
                                 "the application messages for more information" ) );
      return;
   } else {
      m_logger << msg::VERBOSE << "Configuration read in binary format"
               << msg::endmsg;
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split( "/" );
   setWindowTitle( tr( "CDA configuration editor - " ) + filePath.back() );

   return;

}

/* Warning looks like is called twice instead one, but why */
void ConfigEditorWindow::writeXMLConfig( const QString& filename ) {

   //
   // Create a DOM document:
   //
   QDomImplementation imp;
   QDomDocument doc = imp.createDocument( "", "Setup",
                                          imp.createDocumentType( "CDA_SETUP",
                                                                  "", "" ) );

   //
   // Write the configuration to this document:
   //
   QDomElement elem = doc.documentElement();
   if( ! m_devEdit->writeConfig( elem ) ) {
      m_logger << msg::ERROR << "Some error happened while creating the "
               << "XML configuration" << msg::endmsg;
      QMessageBox::critical( this, tr( "Configuration writing error" ),
                             tr( "The configuration could not be written. See "
                                 "the application messages for more information." ) );
      return;
   } else {
      m_logger << msg::VERBOSE << "Configuration translated to XML format"
               << msg::endmsg;
   }

   //
   // Write the document to a file:
   //
   QFile output_file( filename );
   if( ! output_file.open( QFile::WriteOnly | QFile::Truncate ) ) {
      m_logger << msg::ERROR << filename << " couldn't be opened "
               << "for writing" << msg::endmsg;
      QMessageBox::critical( this, tr( "File writing error" ),
                             tr( "The selected file could not be opened for "
                                 "writing. Check permissions!" ) );
      return;
   } else {
      m_logger << msg::DEBUG << filename << " opened for writing"
               << msg::endmsg;
   }

   QTextStream stream( &output_file );
   doc.save( stream, 3 );

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split( "/" );
   setWindowTitle( tr( "CDA configuration editor - " ) + filePath.back() );

   return;

}

void ConfigEditorWindow::writeBinaryConfig( const QString& filename ) {

   //
   // Open the output file:
   //
   QFile output_file( filename );
   if( ! output_file.open( QFile::WriteOnly | QFile::Truncate ) ) {
      m_logger << msg::ERROR << filename << " couldn't be opened "
               << "for writing" << msg::endmsg;
      QMessageBox::critical( this, tr( "File writing error" ),
                             tr( "The selected file could not be opened for "
                                 "writing. Check permissions!" ) );
      return;
   } else {
      m_logger << msg::DEBUG << filename << " opened for writing"
               << msg::endmsg;
   }

   //
   // Write the configuration to this file:
   //
   if( ! m_devEdit->writeConfig( &output_file ) ) {
      m_logger << msg::ERROR << "Some error happened while creating the "
               << "binary configuration" << msg::endmsg;
      QMessageBox::critical( this, tr( "Configuration writing error" ),
                             tr( "The configuration could not be written. See "
                                 "the application messages for more information." ) );
      return;
   } else {
      m_logger << msg::VERBOSE << "Configuration written in binary format"
               << msg::endmsg;
   }

   //
   // Modify the window title:
   //
   QStringList filePath = filename.split( "/" );
   setWindowTitle( tr( "CDA configuration editor - " ) + filePath.back() );

   return;

}
