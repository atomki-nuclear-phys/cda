
// Qt include(s):
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QIcon>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>

// CDA include(s):
#include "common/Address.h"
#include "common/aboutCDA.h"
#include "common/DefaultFont.h"

// Local include(s):
#include "CaenDAQWindow.h"
#include "Constants.h"

/**
 * The constructor takes care of drawing the window, and setting up the
 * interaction between the various Qt objects.
 *
 * @param confFileName Name of the configuration file to be used
 * @param verbosity Output message verbosity of all components
 */
CaenDAQWindow::CaenDAQWindow( const QString& confFileName,
                              msg::Level verbosity )
   : QMainWindow(), m_logger( "CaenDAQWindow" ) {

   // The size of the window depends on the architecture. The menu on MacOS X is
   // not part of the window, so the window can be smaller by 30 pixels...
#ifdef Q_OS_DARWIN
   static const size_t WIDTH = 920;
   static const size_t HEIGHT = 610;
#else
   static const size_t WIDTH = 920;
   static const size_t HEIGHT = 640;
#endif
   resize( WIDTH, HEIGHT );
   setMinimumSize( WIDTH, HEIGHT );
   setMaximumSize( WIDTH, HEIGHT );

   // Set up the window according to whether a configuration file was already
   // specified:
   if( ! confFileName.isEmpty() ) {
      setWindowTitle( tr( "CDA CAEN VME DAQ - %1" ).arg( confFileName ) );
   } else {
      setWindowTitle( tr( "CDA CAEN VME DAQ" ) );
   }
   setWindowIcon( QIcon( ":/img/caen.png" ) );

   //
   // Create the central widget. This is the one on which all other widgets are
   // placed:
   //
   static const size_t W_WIDTH = 920;
   static const size_t W_HEIGHT = 610;
   m_centralWidget.reset( new QWidget( this ) );
   m_centralWidget->resize( W_WIDTH, W_HEIGHT );
   m_centralWidget->setMinimumSize( W_WIDTH, W_HEIGHT );
   m_centralWidget->setMaximumSize( W_WIDTH, W_HEIGHT );
   setCentralWidget( m_centralWidget.get() );

   //
   // Set up the message server and the widget showing the incoming messages:
   //
   m_msgServer.setWriteOutputFile( false );
   m_msgView.reset( new msg::TextView( m_centralWidget.get() ) );
   m_msgView->setGeometry( QRect( 0, 375, 920, 235 ) );
   m_msgView->setMinimumShownLevel( verbosity );
   connect( &m_msgServer, SIGNAL( messageAvailable( const Message& ) ),
            m_msgView.get(), SLOT( addMessage( const Message& ) ) );
   if( ! m_msgServer.listen( Address( MSG_SERVER_ADDRESS ) ) ) {
      QMessageBox::critical( this, tr( "Error starting message server" ),
                             tr( "The TCP/IP message server could not be "
                                 "started on address %1. The application has "
                                 "to shut down!" )
                             .arg( MSG_SERVER_ADDRESS ) );
      QApplication::exit( 1 );
   } else {
      m_logger << msg::INFO << tr( "The message server started successfully" )
               << msg::endmsg;
   }

   //
   // Create the widget showing simple DAQ statistics:
   //
   m_statistics.reset( new simple_daq::Statistics( m_centralWidget.get() ) );
   m_statistics->setGeometry( QRect( 5, 5, 300, 150 ) );
   m_statistics->setStatAddress( Address( STAT_SERVER_ADDRESS ) );

   //
   // Create the widget controlling cda-caen-vme-reader:
   //
   m_caenVmeReader.reset(
            new simple_daq::CaenVmeReaderRunner( m_centralWidget.get() ) );
   m_caenVmeReader->setGeometry( QRect( 310, 5, 300, 150 ) );
   m_caenVmeReader->setConfigFileName( confFileName );
   m_caenVmeReader->setMsgServerAddress( MSG_SERVER_ADDRESS );
   m_caenVmeReader->setStatServerAddress( true, STAT_SERVER_ADDRESS );
   m_caenVmeReader->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_caenVmeReader->setEnabled( false );
   }

   //
   // Create the widget controlling cda-glomem-writer:
   //
   m_glomemWriter.reset(
            new simple_daq::GlomemWriterRunner( m_centralWidget.get() ) );
   m_glomemWriter->setGeometry( QRect( 615, 5, 300, 150 ) );
   m_glomemWriter->setConfigFileName( confFileName );
   m_glomemWriter->setMsgServerAddress( MSG_SERVER_ADDRESS );
   m_glomemWriter->setEventAddress( GLOMEM_WRITER_ADDRESS );
   m_glomemWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_glomemWriter->setEnabled( false );
   }
   connect( m_glomemWriter.get(),
            SIGNAL( receiverRunning( bool, const QString& ) ),
            m_caenVmeReader.get(),
            SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widget controlling cda-hbook-writer:
   //
   m_hbookWriter.reset(
            new simple_daq::HBookWriterRunner( m_centralWidget.get() ) );
   m_hbookWriter->setGeometry( QRect( 5, 160, 300, 150 ) );
   m_hbookWriter->setConfigFileName( confFileName );
   m_hbookWriter->setMsgServerAddress( MSG_SERVER_ADDRESS );
   m_hbookWriter->setEventAddress( HBOOK_WRITER_ADDRESS );
   m_hbookWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_hbookWriter->setEnabled( false );
   }
   connect( m_hbookWriter.get(),
            SIGNAL( receiverRunning( bool, const QString& ) ),
            m_caenVmeReader.get(),
            SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widget controlling cda-root-writer:
   //
   m_rootWriter.reset(
            new simple_daq::RootWriterRunner( m_centralWidget.get() ) );
   m_rootWriter->setGeometry( QRect( 310, 160, 300, 150 ) );
   m_rootWriter->setConfigFileName( confFileName );
   m_rootWriter->setMsgServerAddress( MSG_SERVER_ADDRESS );
   m_rootWriter->setEventAddress( ROOT_WRITER_ADDRESS );
   m_rootWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_rootWriter->setEnabled( false );
   }
   connect( m_rootWriter.get(),
            SIGNAL( receiverRunning( bool, const QString& ) ),
            m_caenVmeReader.get(),
            SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widget controlling cda-raw-writer:
   //
   m_rawWriter.reset(
            new simple_daq::RawWriterRunner( m_centralWidget.get() ) );
   m_rawWriter->setGeometry( QRect( 615, 160, 300, 150 ) );
   m_rawWriter->setConfigFileName( confFileName );
   m_rawWriter->setMsgServerAddress( MSG_SERVER_ADDRESS );
   m_rawWriter->setEventAddress( RAW_WRITER_ADDRESS );
   m_rawWriter->setVerbosity( verbosity );
   if( confFileName.isEmpty() ) {
      m_rawWriter->setEnabled( false );
   }
   connect( m_rawWriter.get(),
            SIGNAL( receiverRunning( bool, const QString& ) ),
            m_caenVmeReader.get(),
            SLOT( setWriterRunning( bool, const QString& ) ) );

   //
   // Create the widgets for specifying extra event receivers:
   //
   m_eventReceiversLabel.reset( new QLabel( tr( "Extra event receivers:" ),
                                            m_centralWidget.get() ) );
   m_eventReceiversLabel->setGeometry( QRect( 50, 315, 200, 25 ) );
   m_eventReceiversLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
   if( confFileName.isEmpty() ) {
      m_eventReceiversLabel->setEnabled( false );
   }

   m_eventReceivers.reset( new QComboBox( m_centralWidget.get() ) );
   m_eventReceivers->setGeometry( QRect( 260, 315, 200, 25 ) );
   m_eventReceivers->setEditable( true );
   m_eventReceivers->setDuplicatesEnabled( false );
   if( confFileName.isEmpty() ) {
      m_eventReceivers->setEnabled( false );
   }
   connect( m_eventReceivers.get(),
            SIGNAL( currentIndexChanged( const QString& ) ),
            this, SLOT( eventReceiverIndexChangedSlot( const QString& ) ) );
   connect( m_eventReceivers.get(), SIGNAL( editTextChanged( const QString& ) ),
            this, SLOT( eventReceiverTextChangedSlot( const QString& ) ) );

   m_currentEventReceiver = m_eventReceivers->currentText();

   m_removeEventReceiver.reset(
            new QPushButton( QIcon::fromTheme( "list-remove" ),
                             tr( "Remove" ), m_centralWidget.get() ) );
   m_removeEventReceiver->setGeometry( QRect( 470, 315, 100, 25 ) );
   if( confFileName.isEmpty() ) {
      m_removeEventReceiver->setEnabled( false );
   }
   connect( m_removeEventReceiver.get(), SIGNAL( pressed() ),
            this, SLOT( removeEventReceiverSlot() ) );

   //
   // Create the widgets for specifying extra statistics receivers:
   //
   m_statReceiversLabel.reset( new QLabel( tr( "Extra statistics receivers:" ),
                                           m_centralWidget.get() ) );
   m_statReceiversLabel->setGeometry( QRect( 50, 345, 200, 25 ) );
   m_statReceiversLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );
   if( confFileName.isEmpty() ) {
      m_statReceiversLabel->setEnabled( false );
   }

   m_statReceivers.reset( new QComboBox( m_centralWidget.get() ) );
   m_statReceivers->setGeometry( QRect( 260, 345, 200, 25 ) );
   m_statReceivers->setEditable( true );
   m_statReceivers->setDuplicatesEnabled( false );
   if( confFileName.isEmpty() ) {
      m_statReceivers->setEnabled( false );
   }
   connect( m_statReceivers.get(),
            SIGNAL( currentIndexChanged( const QString& ) ),
            this, SLOT( statReceiverIndexChangedSlot( const QString& ) ) );
   connect( m_statReceivers.get(), SIGNAL( editTextChanged( const QString& ) ),
            this, SLOT( statReceiverTextChangedSlot( const QString& ) ) );

   m_currentStatReceiver = m_statReceivers->currentText();

   m_removeStatReceiver.reset(
            new QPushButton( QIcon::fromTheme( "list-remove" ),
                             tr( "Remove" ), m_centralWidget.get() ) );
   m_removeStatReceiver->setGeometry( QRect( 470, 345, 100, 25 ) );
   if( confFileName.isEmpty() ) {
      m_removeStatReceiver->setEnabled( false );
   }
   connect( m_removeStatReceiver.get(), SIGNAL( pressed() ),
            this, SLOT( removeStatReceiverSlot() ) );

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
 * This "Qt slot" opens a file selection window and then sets up all the
 * components of the window to use this new selected file.
 */
void CaenDAQWindow::readConfigSlot() {

   // Ask the user for a file name:
   QString fileName =
         QFileDialog::getOpenFileName( this, tr( "Open setup file" ),
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
   setWindowTitle( tr( "CDA CAEN VME DAQ - %1" ).arg( fileName ) );
   m_caenVmeReader->setConfigFileName( fileName );
   m_caenVmeReader->setEnabled( true );
   m_glomemWriter->setConfigFileName( fileName );
   m_glomemWriter->setEnabled( true );
   m_hbookWriter->setConfigFileName( fileName );
   m_hbookWriter->setEnabled( true );
   m_rootWriter->setConfigFileName( fileName );
   m_rootWriter->setEnabled( true );
   m_rawWriter->setConfigFileName( fileName );
   m_rawWriter->setEnabled( true );

   m_eventReceiversLabel->setEnabled( true );
   m_eventReceivers->setEnabled( true );
   m_removeEventReceiver->setEnabled( true );

   m_statReceiversLabel->setEnabled( true );
   m_statReceivers->setEnabled( true );
   m_removeStatReceiver->setEnabled( true );

   m_logger << msg::INFO << tr( "Using configuration file: %1" ).arg( fileName )
            << msg::endmsg;

   return;
}

void CaenDAQWindow::aboutCaenDAQSlot() {

   QMessageBox::about( this, tr( "CDA CAEN VME DAQ" ),
                       tr( "This application is a simplified interface for "
                           "running a CDA CAEN VME data acquisition session. "
                           "While the CDA executables can be started on "
                           "multiple separate computers, this application "
                           "starts one instance of the command line CDA "
                           "applications that can be used in CAEN data "
                           "taking, on the local computer." ) );
   QApplication::setFont( gui::DefaultFont() );

   return;
}

void CaenDAQWindow::aboutCDASlot() {

   aboutCDA( this );
   return;
}

void CaenDAQWindow::eventReceiverIndexChangedSlot( const QString& text ) {

   // Remember the currently selected address:
   m_currentEventReceiver = text;

   return;
}

void CaenDAQWindow::eventReceiverTextChangedSlot( const QString& text ) {

   // Remove the previous address from the event receiver list:
   m_caenVmeReader->setWriterRunning( false, m_currentEventReceiver );
   // Add the current address as an event receiver:
   m_caenVmeReader->setWriterRunning( true, text );
   // Update the current event receiver address:
   m_currentEventReceiver = text;

   return;
}

void CaenDAQWindow::removeEventReceiverSlot() {

   // Remove the event receiver from the CAEN reader runner:
   m_caenVmeReader->setWriterRunning( false, m_eventReceivers->currentText() );

   if( m_eventReceivers->count() > 1 ) {
      // Remove the current entry from the list:
      m_eventReceivers->removeItem( m_eventReceivers->currentIndex() );
      m_currentEventReceiver = m_eventReceivers->currentText();
   } else {
      // Just clear the last entry:
      m_eventReceivers->setItemText( m_eventReceivers->currentIndex(),
                                     "" );
      m_currentEventReceiver = "";
   }

   return;
}

void CaenDAQWindow::statReceiverIndexChangedSlot( const QString& text ) {

   // Remember the currently selected address:
   m_currentStatReceiver = text;

   return;
}

void CaenDAQWindow::statReceiverTextChangedSlot( const QString& text ) {

   // Remove the previous address from the event receiver list:
   m_caenVmeReader->setStatServerAddress( false, m_currentStatReceiver );
   m_glomemWriter->setStatServerAddress( false, m_currentStatReceiver );
   m_hbookWriter->setStatServerAddress( false, m_currentStatReceiver );
   m_rootWriter->setStatServerAddress( false, m_currentStatReceiver );
   m_rawWriter->setStatServerAddress( false, m_currentStatReceiver );
   // Add the current address as an event receiver:
   m_caenVmeReader->setStatServerAddress( true, text );
   m_glomemWriter->setStatServerAddress( true, text );
   m_hbookWriter->setStatServerAddress( true, text );
   m_rootWriter->setStatServerAddress( true, text );
   m_rawWriter->setStatServerAddress( true, text );
   // Update the current event receiver address:
   m_currentStatReceiver = text;

   return;
}

void CaenDAQWindow::removeStatReceiverSlot() {

   // Remove the current entry from the "runners":
   m_caenVmeReader->setStatServerAddress( false,
                                          m_statReceivers->currentText() );
   m_glomemWriter->setStatServerAddress( false,
                                         m_statReceivers->currentText() );
   m_hbookWriter->setStatServerAddress( false, m_statReceivers->currentText() );
   m_rootWriter->setStatServerAddress( false, m_statReceivers->currentText() );
   m_rawWriter->setStatServerAddress( false, m_statReceivers->currentText() );

   if( m_statReceivers->count() > 1 ) {
      // Remove the current entry from the list:
      m_statReceivers->removeItem( m_statReceivers->currentIndex() );
      m_currentStatReceiver = m_statReceivers->currentText();
   } else {
      // Just clear the last entry:
      m_statReceivers->setItemText( m_statReceivers->currentIndex(),
                                    "" );
      m_currentStatReceiver = "";
   }

   return;
}

/**
 * This function is responsible for creating a few simple menus on the top of
 * the window.
 */
void CaenDAQWindow::drawMenus() {

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
   QMenu* helpMenu = menuBar()->addMenu( tr( "&Help" ) );

   QAction* aboutQtAction = helpMenu->addAction( QIcon( ":/img/qt-logo.jpg" ),
                                                 tr( "About Qt" ) );
   connect( aboutQtAction, SIGNAL( triggered() ),
            qApp, SLOT( aboutQt() ) );

   QAction* aboutCaenDaqAc =
      helpMenu->addAction( QIcon( ":/img/caen.png" ),
                           tr( "About Caen DAQ" ) );
   connect( aboutCaenDaqAc, SIGNAL( triggered() ),
            this, SLOT( aboutCaenDAQSlot() ) );

   QAction* aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                                  tr( "About CDA" ) );
   connect( aboutCDAAction, SIGNAL( triggered() ),
            this, SLOT( aboutCDASlot() ) );

   return;
}
