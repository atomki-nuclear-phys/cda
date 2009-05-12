// $Id$

// Qt include(s):
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtGui/QIcon>
#include <QtGui/QMenuBar>
#include <QtGui/QMenu>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdacore/msg/Server.h"
#   include "cdagui/msg/TextView.h"
#   include "cdagui/simple_daq/Statistics.h"
#   include "cdagui/simple_daq/CamacReaderRunner.h"
#   include "cdagui/simple_daq/GlomemWriterRunner.h"
#   include "cdagui/simple_daq/HBookWriterRunner.h"
#else
#   include "common/Address.h"
#   include "msg/Server.h"
#   include "msg/TextView.h"
#   include "simple_daq/Statistics.h"
#   include "simple_daq/CamacReaderRunner.h"
#   include "simple_daq/GlomemWriterRunner.h"
#   include "simple_daq/HBookWriterRunner.h"
#endif

// Local include(s):
#include "SimpleDAQWindow.h"
#include "Constants.h"

SimpleDAQWindow::SimpleDAQWindow( const QString& confFileName, msg::Level verbosity )
   : QMainWindow(), m_logger( "SimpleDAQWindow" ) {

#ifdef Q_OS_DARWIN
   resize( 615, 550 );
   setMinimumSize( 615, 550 );
   setMaximumSize( 615, 550 );
#else
   resize( 615, 575 );
   setMinimumSize( 615, 575 );
   setMaximumSize( 615, 575 );
#endif

   if( ! confFileName.isEmpty() ) {
      setWindowTitle( tr( "CDA Simple DAQ - %1" ).arg( confFileName ) );
   } else {
      setWindowTitle( tr( "CDA Simple DAQ" ) );
   }
   setWindowIcon( QIcon( ":/img/logo.png" ) );

   m_centralWidget = new QWidget( this );
   m_centralWidget->resize( 615, 550 );
   m_centralWidget->setMinimumSize( 615, 550 );
   m_centralWidget->setMaximumSize( 615, 550 );
   setCentralWidget( m_centralWidget );

   m_msgServer = new msg::Server();
   m_msgServer->setWriteOutputFile( false );
   m_msgView = new msg::TextView( m_centralWidget );
   m_msgView->setGeometry( QRect( 0, 315, 615, 235 ) );
   m_msgView->setMinimumShownLevel( verbosity );
   connect( m_msgServer, SIGNAL( messageAvailable( const Message& ) ),
            m_msgView, SLOT( addMessage( const Message& ) ) );

   if( ! m_msgServer->listen( Address( Const::MSG_SERVER_ADDRESS ) ) ) {
      QMessageBox::critical( this, tr( "Error starting message server" ),
                             tr( "The TCP/IP message server could not be started "
                                 "on address %1. The application has to shut down!" )
                             .arg( Const::MSG_SERVER_ADDRESS ) );
      QCoreApplication::exit( 1 );
   } else {
      m_logger << msg::INFO << tr( "The message server started successfully" )
               << msg::endmsg;
   }

   m_statistics = new simple_daq::Statistics( m_centralWidget );
   m_statistics->setGeometry( QRect( 5, 5, 300, 150 ) );
   m_statistics->setStatAddress( Address( Const::STAT_SERVER_ADDRESS ) );

   m_camacReader = new simple_daq::CamacReaderRunner( m_centralWidget );
   m_camacReader->setGeometry( QRect( 310, 5, 300, 150 ) );
   m_camacReader->setConfigFileName( confFileName );
   m_camacReader->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_camacReader->setStatServerAddress( Const::STAT_SERVER_ADDRESS );
   m_camacReader->setHBookWriterAddress( Const::HBOOK_WRITER_ADDRESS );
   m_camacReader->setGlomemWriterAddress( Const::GLOMEM_WRITER_ADDRESS );
   m_camacReader->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_camacReader->setEnabled( false );
   }

   m_glomemWriter = new simple_daq::GlomemWriterRunner( m_centralWidget );
   m_glomemWriter->setGeometry( QRect( 5, 160, 300, 150 ) );
   m_glomemWriter->setConfigFileName( confFileName );
   m_glomemWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_glomemWriter->setEventAddress( Const::GLOMEM_WRITER_ADDRESS );
   m_glomemWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_glomemWriter->setEnabled( false );
   }

   connect( m_glomemWriter, SIGNAL( running( bool ) ),
            m_camacReader, SLOT( setGlomemWriterRunning( bool ) ) );

   m_hbookWriter = new simple_daq::HBookWriterRunner( m_centralWidget );
   m_hbookWriter->setGeometry( QRect( 310, 160, 300, 150 ) );
   m_hbookWriter->setConfigFileName( confFileName );
   m_hbookWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_hbookWriter->setEventAddress( Const::HBOOK_WRITER_ADDRESS );
   m_hbookWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_hbookWriter->setEnabled( false );
   }

   connect( m_hbookWriter, SIGNAL( running( bool ) ),
            m_camacReader, SLOT( setHBookWriterRunning( bool ) ) );

   drawMenus();

}

SimpleDAQWindow::~SimpleDAQWindow() {

   delete m_statistics;
   delete m_camacReader;
   delete m_msgServer;
   delete m_msgView;
   delete m_centralWidget;

}

void SimpleDAQWindow::readConfigSlot() {

   // Ask the user for a file name:
   QString fileName = QFileDialog::getOpenFileName( this, tr( "Open setup file" ),
                                                    tr( "default.cxml" ),
                                                    tr( "CDA XML setup files (*.cxml);;"
                                                        "All files (*)" ) );
   if( fileName.isEmpty() ) {
      m_logger << msg::INFO << "Configuration file selection cancelled" << msg::endmsg;
      return;
   }

   setWindowTitle( tr( "CDA Simple DAQ - %1" ).arg( fileName ) );
   m_camacReader->setConfigFileName( fileName );
   m_camacReader->setEnabled( true );
   m_glomemWriter->setConfigFileName( fileName );
   m_glomemWriter->setEnabled( true );
   m_hbookWriter->setConfigFileName( fileName );
   m_hbookWriter->setEnabled( true );

   m_logger << msg::INFO << "Using configuration file: "
            << fileName << msg::endmsg;

   return;

}

void SimpleDAQWindow::aboutQtSlot() {

   QMessageBox::aboutQt( this, tr( "CDA Simple DAQ - built on Qt" ) );
   return;

}

void SimpleDAQWindow::aboutSimpleDAQSlot() {

   QMessageBox::about( this, tr( "CDA Simple DAQ" ),
                       tr( "This application is a simplified interface for running "
                           "a CDA data acquisition session. While the CDA "
                           "executables can be started on multiple separate computers, "
                           "this application starts one instance of the 3 main CDA "
                           "applications on the local computer." ) );

   return;

}

void SimpleDAQWindow::aboutCDASlot() {

   QMessageBox::about( this, tr( "CDA - CAMAC Data Acquisition" ),
                       tr( "CDA is an application suit built for the "
                           "Experimental Nuclear Physics department of the "
                           "Atomki. It is composed of multiple executables "
                           "that work together in the data acquisition." ) );
   return;

}

void SimpleDAQWindow::drawMenus() {

   /////////////////////////////////////////////////////////////
   //                                                         //
   //                 Create the File menu                    //
   //                                                         //
   /////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu( tr( "&File" ) );

   QAction* openAction = fileMenu->addAction( QIcon( ":/img/fileopen.png" ),
                                              tr( "&Open configuration file..." ) );
   connect( openAction, SIGNAL( triggered() ),
            this, SLOT( readConfigSlot() ) );

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

   QAction* aboutConfigEditorAc = helpMenu->addAction( tr( "About Simple DAQ" ) );
   connect( aboutConfigEditorAc, SIGNAL( triggered() ),
            this, SLOT( aboutSimpleDAQSlot() ) );

   QAction* aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                                  tr( "About CDA" ) );
   connect( aboutCDAAction, SIGNAL( triggered() ),
            this, SLOT( aboutCDASlot() ) );

   return;

}
