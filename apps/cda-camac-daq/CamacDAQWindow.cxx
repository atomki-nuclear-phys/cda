// $Id$

// Qt include(s):
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtGui/QApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdacore/msg/Server.h"
#   include "cdagui/msg/TextView.h"
#   include "cdagui/simple_daq/Statistics.h"
#   include "cdagui/simple_daq/CamacReaderRunner.h"
#   include "cdagui/simple_daq/GlomemWriterRunner.h"
#   include "cdagui/simple_daq/HBookWriterRunner.h"
#   include "cdagui/simple_daq/RootWriterRunner.h"
#   include "cdagui/simple_daq/RawWriterRunner.h"
#   include "cdagui/common/aboutCDA.h"
#   include "cdagui/common/DefaultFont.h"
#else
#   include "common/Address.h"
#   include "msg/Server.h"
#   include "msg/TextView.h"
#   include "simple_daq/Statistics.h"
#   include "simple_daq/CamacReaderRunner.h"
#   include "simple_daq/GlomemWriterRunner.h"
#   include "simple_daq/HBookWriterRunner.h"
#   include "simple_daq/RootWriterRunner.h"
#   include "simple_daq/RawWriterRunner.h"
#   include "common/aboutCDA.h"
#   include "common/DefaultFont.h"
#endif

// Local include(s):
#include "CamacDAQWindow.h"
#include "Constants.h"

/**
 * The constructor takes care of drawing the window, and setting up the interaction
 * between the various Qt objects.
 *
 * @param confFileName Name of the configuration file to be used
 * @param verbosity Output message verbosity of all components
 */
CamacDAQWindow::CamacDAQWindow( const QString& confFileName, msg::Level verbosity )
   : QMainWindow(), m_logger( "CamacDAQWindow" ) {

   // The size of the window depends on the architecture. The menu on MacOS X is not part
   // of the window, so the window can be smaller by 30 pixels...
#ifdef Q_OS_DARWIN
   resize( 920, 550 );
   setMinimumSize( 920, 550 );
   setMaximumSize( 920, 550 );
#else
   resize( 920, 580 );
   setMinimumSize( 920, 580 );
   setMaximumSize( 920, 580 );
#endif

   // Set up the window according to whether a configuration file was already specified:
   if( ! confFileName.isEmpty() ) {
      setWindowTitle( tr( "CDA CAMAC DAQ - %1" ).arg( confFileName ) );
   } else {
      setWindowTitle( tr( "CDA CAMAC DAQ" ) );
   }
   setWindowIcon( QIcon( ":/img/cda-daq.png" ) );

   //
   // Create the central widget. This is the one on which all other widgets are placed:
   //
   m_centralWidget = new QWidget( this );
   m_centralWidget->resize( 920, 550 );
   m_centralWidget->setMinimumSize( 920, 550 );
   m_centralWidget->setMaximumSize( 920, 550 );
   setCentralWidget( m_centralWidget );

   //
   // Set up the message server and the widget showing the incoming messages:
   //
   m_msgServer = new msg::Server();
   m_msgServer->setWriteOutputFile( false );
   m_msgView = new msg::TextView( m_centralWidget );
   m_msgView->setGeometry( QRect( 0, 315, 920, 235 ) );
   m_msgView->setMinimumShownLevel( verbosity );
   connect( m_msgServer, SIGNAL( messageAvailable( const Message& ) ),
            m_msgView, SLOT( addMessage( const Message& ) ) );
   if( ! m_msgServer->listen( Address( Const::MSG_SERVER_ADDRESS ) ) ) {
      QMessageBox::critical( this, tr( "Error starting message server" ),
                             tr( "The TCP/IP message server could not be started "
                                 "on address %1. The application has to shut down!" )
                             .arg( Const::MSG_SERVER_ADDRESS ) );
      QApplication::exit( 1 );
   } else {
      m_logger << msg::INFO << tr( "The message server started successfully" )
               << msg::endmsg;
   }

   //
   // Create the widget showing simple DAQ statistics:
   //
   m_statistics = new simple_daq::Statistics( m_centralWidget );
   m_statistics->setGeometry( QRect( 5, 5, 300, 150 ) );
   m_statistics->setStatAddress( Address( Const::STAT_SERVER_ADDRESS ) );

   //
   // Create the widget controlling cda-camac-reader:
   //
   m_camacReader = new simple_daq::CamacReaderRunner( m_centralWidget );
   m_camacReader->setGeometry( QRect( 310, 5, 300, 150 ) );
   m_camacReader->setConfigFileName( confFileName );
   m_camacReader->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_camacReader->setStatServerAddress( Const::STAT_SERVER_ADDRESS );
   m_camacReader->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_camacReader->setEnabled( false );
   }

   //
   // Create the widget controlling cda-glomem-writer:
   //
   m_glomemWriter = new simple_daq::GlomemWriterRunner( m_centralWidget );
   m_glomemWriter->setGeometry( QRect( 615, 5, 300, 150 ) );
   m_glomemWriter->setConfigFileName( confFileName );
   m_glomemWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_glomemWriter->setEventAddress( Const::GLOMEM_WRITER_ADDRESS );
   m_glomemWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_glomemWriter->setEnabled( false );
   }
   connect( m_glomemWriter, SIGNAL( receiverRunning( bool, const QString& ) ),
            m_camacReader, SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widget controlling cda-hbook-writer:
   //
   m_hbookWriter = new simple_daq::HBookWriterRunner( m_centralWidget );
   m_hbookWriter->setGeometry( QRect( 5, 160, 300, 150 ) );
   m_hbookWriter->setConfigFileName( confFileName );
   m_hbookWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_hbookWriter->setEventAddress( Const::HBOOK_WRITER_ADDRESS );
   m_hbookWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_hbookWriter->setEnabled( false );
   }
   connect( m_hbookWriter, SIGNAL( receiverRunning( bool, const QString& ) ),
            m_camacReader, SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widget controlling cda-root-writer:
   //
   m_rootWriter = new simple_daq::RootWriterRunner( m_centralWidget );
   m_rootWriter->setGeometry( QRect( 310, 160, 300, 150 ) );
   m_rootWriter->setConfigFileName( confFileName );
   m_rootWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_rootWriter->setEventAddress( Const::ROOT_WRITER_ADDRESS );
   m_rootWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_rootWriter->setEnabled( false );
   }
   connect( m_rootWriter, SIGNAL( receiverRunning( bool, const QString& ) ),
            m_camacReader, SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widget controlling cda-raw-writer:
   //
   m_rawWriter = new simple_daq::RawWriterRunner( m_centralWidget );
   m_rawWriter->setGeometry( QRect( 615, 160, 300, 150 ) );
   m_rawWriter->setConfigFileName( confFileName );
   m_rawWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_rawWriter->setEventAddress( Const::RAW_WRITER_ADDRESS );
   m_rawWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_rawWriter->setEnabled( false );
   }
   connect( m_rawWriter, SIGNAL( receiverRunning( bool, const QString& ) ),
            m_camacReader, SLOT( setWriterRunning( bool, const QString& ) ) );

   // Draw the menus of the window:
   drawMenus();

   //
   // Print some final log message:
   //
   if( ! confFileName.isEmpty() ) {
      m_logger << msg::INFO
               << tr( "Using configuration file: %1" ).arg( confFileName )
               << msg::endmsg;
   } else {
      m_logger << msg::INFO << tr( "Please open a configuration file before"
                                   " trying to start the DAQ system." )
               << msg::endmsg;
   }

}

/**
 * The destructor actually does something in this class, it has to delete quite a few
 * objects which were created in the constructor.
 */
CamacDAQWindow::~CamacDAQWindow() {

   //
   // Delete all objects created in the constructor:
   //
   delete m_statistics;
   delete m_camacReader;
   delete m_glomemWriter;
   delete m_hbookWriter;
   delete m_rootWriter;
   delete m_rawWriter;
   delete m_msgServer;
   delete m_msgView;
   delete m_centralWidget;
}

/**
 * This "Qt slot" opens a file selection window and then sets up all the components of the
 * window to use this new selected file.
 */
void CamacDAQWindow::readConfigSlot() {

   // Ask the user for a file name:
   QString fileName = QFileDialog::getOpenFileName( this, tr( "Open setup file" ),
                                                    tr( "default.cxml" ),
                                                    tr( "CDA XML setup files (*.cxml);;"
                                                        "All files (*)" ) );
   QApplication::setFont( gui::DefaultFont() );

   // If the user cancels the file selection, don't continue:
   if( fileName.isEmpty() ) {
      m_logger << msg::INFO << tr( "Configuration file selection cancelled" )
               << msg::endmsg;
      return;
   }

   //
   // Set up everything to use this new configuration file:
   //
   setWindowTitle( tr( "CDA CAMAC DAQ - %1" ).arg( fileName ) );
   m_camacReader->setConfigFileName( fileName );
   m_camacReader->setEnabled( true );
   m_glomemWriter->setConfigFileName( fileName );
   m_glomemWriter->setEnabled( true );
   m_hbookWriter->setConfigFileName( fileName );
   m_hbookWriter->setEnabled( true );
   m_rootWriter->setConfigFileName( fileName );
   m_rootWriter->setEnabled( true );
   m_rawWriter->setConfigFileName( fileName );
   m_rawWriter->setEnabled( true );

   m_logger << msg::INFO << tr( "Using configuration file: %1" ).arg( fileName )
            << msg::endmsg;

   return;
}

void CamacDAQWindow::aboutCamacDAQSlot() {

   QMessageBox::about( this, tr( "CDA Camac DAQ" ),
                       tr( "This application is a simplified interface for running "
                           "a CDA CAMAC data acquisition session. While the CDA "
                           "executables can be started on multiple separate computers, "
                           "this application starts one instance of the 3 CDA "
                           "applications used in CAMAC data taking, on the "
                           "local computer." ) );
   QApplication::setFont( gui::DefaultFont() );

   return;
}

void CamacDAQWindow::aboutCDASlot() {

   aboutCDA( this );
   return;
}

/**
 * This function is responsible for creating a few simple menus on the top of the window.
 */
void CamacDAQWindow::drawMenus() {

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the File menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu( tr( "&File" ) );

   QAction* openAction =
      fileMenu->addAction( QIcon::fromTheme( "document-open",
                                             QIcon( ":/img/fileopen.png" ) ),
                           tr( "&Open configuration file..." ) );
   connect( openAction, SIGNAL( triggered() ),
            this, SLOT( readConfigSlot() ) );

   fileMenu->addSeparator();

   QAction* quitAction =
      fileMenu->addAction( QIcon::fromTheme( "application-exit",
                                             QIcon( ":/img/warning.png" ) ),
                           tr( "&Quit" ) );
   connect( quitAction, SIGNAL( triggered() ),
            this, SLOT( close() ) );

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the Help menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu( QIcon::fromTheme( "system-help" ),
                                         tr( "&Help" ) );

   QAction* aboutQtAction = helpMenu->addAction( QIcon( ":/img/qt-logo.jpg" ),
                                                 tr( "About Qt" ) );
   connect( aboutQtAction, SIGNAL( triggered() ),
            qApp, SLOT( aboutQt() ) );

   QAction* aboutConfigEditorAc =
      helpMenu->addAction( QIcon( ":/img/cda-daq.png" ),
                           tr( "About Camac DAQ" ) );
   connect( aboutConfigEditorAc, SIGNAL( triggered() ),
            this, SLOT( aboutCamacDAQSlot() ) );

   QAction* aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                                  tr( "About CDA" ) );
   connect( aboutCDAAction, SIGNAL( triggered() ),
            this, SLOT( aboutCDASlot() ) );

   return;
}