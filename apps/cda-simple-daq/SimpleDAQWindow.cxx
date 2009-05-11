// $Id$

// Qt include(s):
#include <QtGui/QWidget>
#include <QtGui/QMessageBox>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Server.h"
#   include "cdagui/msg/TextView.h"
#   include "cdagui/simple_daq/CamacReaderRunner.h"
#   include "cdagui/simple_daq/GlomemWriterRunner.h"
#   include "cdagui/simple_daq/HBookWriterRunner.h"
#else
#   include "msg/Server.h"
#   include "msg/TextView.h"
#   include "simple_daq/CamacReaderRunner.h"
#   include "simple_daq/GlomemWriterRunner.h"
#   include "simple_daq/HBookWriterRunner.h"
#endif

// Local include(s):
#include "SimpleDAQWindow.h"
#include "Constants.h"

SimpleDAQWindow::SimpleDAQWindow( const QString& confFileName, msg::Level verbosity )
   : QMainWindow(), m_logger( "SimpleDAQWindow" ) {

   resize( 615, 550 );
   setMinimumSize( 615, 550 );
   setMaximumSize( 615, 550 );

   setWindowTitle( tr( "CDA Simple DAQ - %1" ).arg( confFileName ) );

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

   m_camacReader = new simple_daq::CamacReaderRunner( m_centralWidget );
   m_camacReader->setGeometry( QRect( 310, 5, 300, 150 ) );
   m_camacReader->setConfigFileName( confFileName );
   m_camacReader->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_camacReader->setHBookWriterAddress( Const::HBOOK_WRITER_ADDRESS );
   m_camacReader->setGlomemWriterAddress( Const::GLOMEM_WRITER_ADDRESS );
   m_camacReader->setVerbosity( verbosity );

   m_glomemWriter = new simple_daq::GlomemWriterRunner( m_centralWidget );
   m_glomemWriter->setGeometry( QRect( 5, 160, 300, 150 ) );
   m_glomemWriter->setConfigFileName( confFileName );
   m_glomemWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_glomemWriter->setEventAddress( Const::GLOMEM_WRITER_ADDRESS );
   m_glomemWriter->setVerbosity( verbosity );

   connect( m_glomemWriter, SIGNAL( running( bool ) ),
            m_camacReader, SLOT( setGlomemWriterRunning( bool ) ) );

   m_hbookWriter = new simple_daq::HBookWriterRunner( m_centralWidget );
   m_hbookWriter->setGeometry( QRect( 310, 160, 300, 150 ) );
   m_hbookWriter->setConfigFileName( confFileName );
   m_hbookWriter->setMsgServerAddress( Const::MSG_SERVER_ADDRESS );
   m_hbookWriter->setEventAddress( Const::HBOOK_WRITER_ADDRESS );
   m_hbookWriter->setVerbosity( verbosity );

   connect( m_hbookWriter, SIGNAL( running( bool ) ),
            m_camacReader, SLOT( setHBookWriterRunning( bool ) ) );

}

SimpleDAQWindow::~SimpleDAQWindow() {

   delete m_camacReader;
   delete m_msgServer;
   delete m_msgView;
   delete m_centralWidget;

}
