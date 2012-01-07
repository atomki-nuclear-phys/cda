// $Id$

// Qt include(s):
#include <QtGui/QDockWidget>
#include <QtGui/QSpinBox>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>
#include <QtGui/QStatusBar>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Server.h"
#   include "cdagui/msg/TextView.h"
#   include "cdagui/common/aboutCDA.h"
#else
#   include "msg/Server.h"
#   include "msg/TextView.h"
#   include "common/aboutCDA.h"
#endif

// Local include(s):
#include "MsgServerWindow.h"

/**
 * All the constituents of this window are created on the heap, so they
 * are all initialised in the constructor. It creates the dock widgets, the
 * menus and the message handling objects. Then it of course connects up
 * all these objects to do the right thing.
 *
 * When created, the msg::Server object is told to listen on port 49700
 * on localhost. This can be changed in the appropriate widget later on.
 */
MsgServerWindow::MsgServerWindow()
   : QMainWindow() {

   //
   // Set up the window. Notice, that I assume that the CDA logo is
   // embedded in the executable in the "/img"
   // "virtual directory".
   //
   resize( 750, 500 );
   setWindowTitle( tr( "CDA Message Server" ) );
   setWindowIcon( QIcon( ":/img/cda-msgserver.png" ) );

   //
   // Create the objects doing most of the work:
   //
   m_server = new msg::Server();
   //   m_view   = new msg::TableView();
   m_view   = new msg::TextView();
   setCentralWidget( m_view );

   //
   // Connect up these two objects:
   //
   connect( m_server, SIGNAL( messageAvailable( const Message& ) ),
            m_view, SLOT( addMessage( const Message& ) ) );

   //
   // Create the dock widgets and the menus:
   //
   createDockWidgets();
   createMenus();

   //
   // Set the behaviour of the dock widgets:
   //
   setDockNestingEnabled( true );
   setDockOptions( QMainWindow::AnimatedDocks );

   //
   // Start listening on port 49700 with all network interfaces:
   //
   if( ! m_server->listen( Address( "0.0.0.0", 49700 ) ) ) {
      QMessageBox::critical( this, tr( "Error starting server" ),
                             tr( "The TCP/IP message server could not be "
                                 "started on port %1. Please select another "
                                 "port!" ).arg( 49700 ) );
      statusBar()->showMessage( tr( "The server is stopped" ) );
   } else {
      statusBar()->showMessage( tr( "The server is running on port %1" ).arg( 49700 ) );
   }

}

/**
 * The destructor only has to delete the two message handling objects,
 * all the graphical objects are taken care of by Qt.
 */
MsgServerWindow::~MsgServerWindow() {

   //
   // Only these two are not taken care of by Qt:
   //
   delete m_server;
   delete m_view;

}

/**
 * When the port changes the server is told to stop listening on the
 * previously configured port and is then re-started to listen on the
 * new port.
 */
void MsgServerWindow::portChangedSlot() {

   const int port = m_portEdit->value();
   static int previous_port = 0;
   if( port == previous_port ) {
      return;
   } else {
      previous_port = port;
   }
   if( m_server->isListening() ) m_server->close();
   if( ! m_server->listen( Address( "0.0.0.0", port ) ) ) {
      QMessageBox::critical( this, tr( "Error starting server" ),
                             tr( "The TCP/IP message server could not be "
                                 "started on port %1. Please select another "
                                 "port!" ).arg( port ) );
      statusBar()->showMessage( tr( "The server is stopped" ) );
   } else {
      statusBar()->showMessage( tr( "The server is running on port %1" ).arg( port ) );
   }
   return;

}

void MsgServerWindow::shownLinesChangedSlot() {

   m_view->setMaximumMessageCount( m_shownLinesEdit->value() );
   return;

}

void MsgServerWindow::outputFileWritingChangedSlot( bool value ) {

   m_server->setWriteOutputFile( value );
   return;

}

void MsgServerWindow::outputFileNameChangedSlot( const QString& text ) {

   m_server->setOutputFileName( text );
   return;

}

void MsgServerWindow::minMsgLevelChangedSlot( const QString& value ) {

   if( value == "VERBOSE" ) {
      m_view->setMinimumShownLevel( msg::VERBOSE );
   } else if( value == "DEBUG" ) {
      m_view->setMinimumShownLevel( msg::DEBUG );
   } else if( value == "INFO" ) {
      m_view->setMinimumShownLevel( msg::INFO );
   } else if( value == "WARNING" ) {
      m_view->setMinimumShownLevel( msg::WARNING );
   } else if( value == "ERROR" ) {
      m_view->setMinimumShownLevel( msg::ERROR );
   } else if( value == "FATAL" ) {
      m_view->setMinimumShownLevel( msg::FATAL );
   } else if( value == "ALWAYS" ) {
      m_view->setMinimumShownLevel( msg::ALWAYS );
   } else {
      m_view->setMinimumShownLevel( msg::INFO );
   }

   return;

}

void MsgServerWindow::aboutQtSlot() {

   QMessageBox::aboutQt( this, tr( "CDA Message Server - built on Qt" ) );
   return;

}

void MsgServerWindow::aboutCDASlot() {

   aboutCDA( this );
   return;

}

void MsgServerWindow::aboutMsgServerSlot() {

   QMessageBox::about( this, tr( "CDA Message Server" ),
                       tr( "The CDA Message Server is an application in which "
                           "all parts of CDA can be monitored. It listens for "
                           "incoming messages from the various components and "
                           "presents these to the user in a nice way." ) );
   return;

}

/**
 * This is where the menus are set up. Notice that the function assumes that
 * the dock widgets have already been set up, so this function has to be
 * called after createDockWidgets() has already run.
 */
void MsgServerWindow::createMenus() {

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                    Create a "File" menu                     //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu( tr( "File" ) );

   m_quitAction = fileMenu->addAction( QIcon( ":/img/warning.png" ),
                                       tr( "Quit" ) );
   connect( m_quitAction, SIGNAL( triggered() ),
            this, SLOT( close() ) );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                Create a "Preferences" menu                  //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* prefMenu = menuBar()->addMenu( tr( "Preferences" ) );

   m_showPortDock = prefMenu->addAction( tr( "Show port setting" ) );
   m_showPortDock->setCheckable( true );
   connect( m_showPortDock, SIGNAL( toggled( bool ) ),
            m_portDock, SLOT( setVisible( bool ) ) );
   connect( m_portDock, SIGNAL( visibilityChanged( bool ) ),
            m_showPortDock, SLOT( setChecked( bool ) ) );

   m_showShownLinesDock = prefMenu->addAction( tr( "Show shown lines setting" ) );
   m_showShownLinesDock->setCheckable( true );
   connect( m_showShownLinesDock, SIGNAL( toggled( bool ) ),
            m_shownLinesDock, SLOT( setVisible( bool ) ) );
   connect( m_shownLinesDock, SIGNAL( visibilityChanged( bool ) ),
            m_showShownLinesDock, SLOT( setChecked( bool ) ) );

   m_showMsgLevelDock = prefMenu->addAction( tr( "Show min. level setting" ) );
   m_showMsgLevelDock->setCheckable( true );
   connect( m_showMsgLevelDock, SIGNAL( toggled( bool ) ),
            m_minMsgLevelDock, SLOT( setVisible( bool ) ) );
   connect( m_minMsgLevelDock, SIGNAL( visibilityChanged( bool ) ),
            m_showMsgLevelDock, SLOT( setChecked( bool ) ) );

   m_showOutputFileDock = prefMenu->addAction( tr( "Show output file setting" ) );
   m_showOutputFileDock->setCheckable( true );
   connect( m_showOutputFileDock, SIGNAL( toggled( bool ) ),
            m_outputFileDock, SLOT( setVisible( bool ) ) );
   connect( m_outputFileDock, SIGNAL( visibilityChanged( bool ) ),
            m_showOutputFileDock, SLOT( setChecked( bool ) ) );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                    Create a "Help" menu                     //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu( tr( "Help" ) );

   m_aboutQtAction = helpMenu->addAction( QIcon( ":/img/qt-logo.jpg" ),
                                          tr( "About Qt" ) );
   connect( m_aboutQtAction, SIGNAL( triggered() ),
            this, SLOT( aboutQtSlot() ) );

   m_aboutMsgServerAction =
      helpMenu->addAction( QIcon( ":/img/cda-msgserver.png" ),
                           tr( "About Message Server" ) );
   connect( m_aboutMsgServerAction, SIGNAL( triggered() ),
            this, SLOT( aboutMsgServerSlot() ) );

   m_aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                           tr( "About CDA" ) );
   connect( m_aboutCDAAction, SIGNAL( triggered() ),
            this, SLOT( aboutCDASlot() ) );

   return;

}

/**
 * Create the dock widgets handling all the configurations of the
 * message server and viewer. This function has to be called early
 * in the initialisation of the window, but after the message server
 * and message viewer objects are already created.
 */
void MsgServerWindow::createDockWidgets() {

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //              Create the port editing widgets                //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_portEdit = new QSpinBox();
   m_portEdit->setMinimum( 100 );
   m_portEdit->setMaximum( 80000 );
   m_portEdit->setValue( 49700 );
   connect( m_portEdit, SIGNAL( editingFinished() ),
            this, SLOT( portChangedSlot() ) );

   m_portDock = new QDockWidget( tr( "Server port" ), this );
   m_portDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                Qt::BottomDockWidgetArea );
   m_portDock->setWidget( m_portEdit );

   addDockWidget( Qt::TopDockWidgetArea, m_portDock );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //   Create the widgets setting the number of shown messages   //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_shownLinesEdit = new QSpinBox();
   m_shownLinesEdit->setMinimum( 1 );
   m_shownLinesEdit->setMaximum( 20000 );
   m_shownLinesEdit->setValue( 100 );
   connect( m_shownLinesEdit, SIGNAL( editingFinished() ),
            this, SLOT( shownLinesChangedSlot() ) );

   m_shownLinesDock = new QDockWidget( tr( "Max. number of messages" ), this );
   m_shownLinesDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                      Qt::BottomDockWidgetArea );
   m_shownLinesDock->setWidget( m_shownLinesEdit );

   addDockWidget( Qt::TopDockWidgetArea, m_shownLinesDock );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //  Create the widgets for the minimum message level setting   //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_minMsgLevelEdit = new QComboBox();
   m_minMsgLevelEdit->addItem( "VERBOSE" );
   m_minMsgLevelEdit->addItem( "DEBUG" );
   m_minMsgLevelEdit->addItem( "INFO" );
   m_minMsgLevelEdit->addItem( "WARNING" );
   m_minMsgLevelEdit->addItem( "ERROR" );
   m_minMsgLevelEdit->addItem( "FATAL" );
   m_minMsgLevelEdit->addItem( "ALWAYS" );
   connect( m_minMsgLevelEdit, SIGNAL( currentIndexChanged( const QString& ) ),
            this, SLOT( minMsgLevelChangedSlot( const QString& ) ) );

   m_minMsgLevelDock = new QDockWidget( tr( "Min. message level" ), this );
   m_minMsgLevelDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                       Qt::BottomDockWidgetArea );
   m_minMsgLevelDock->setWidget( m_minMsgLevelEdit );

   addDockWidget( Qt::TopDockWidgetArea, m_minMsgLevelDock );

   //
   // Make sure that none of the dock widgets are dwarfed by the others:
   //
   m_portDock->setMinimumWidth( width() / 3 );
   m_shownLinesDock->setMinimumWidth( width() / 3 );
   m_minMsgLevelDock->setMinimumWidth( width() / 3 );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //       Create the widgets for the output file settings       //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_outputFileWidget = new QWidget();

   m_outputFileVLayout = new QVBoxLayout( m_outputFileWidget );

   m_outputFileBox = new QGroupBox( tr( "Write output file" ) );
   m_outputFileBox->setCheckable( true );
   connect( m_outputFileBox, SIGNAL( toggled( bool ) ),
            this, SLOT( outputFileWritingChangedSlot( bool ) ) );
   m_outputFileBox->setChecked( false );
   m_outputFileVLayout->addWidget( m_outputFileBox );

   m_outputFileLayout = new QHBoxLayout( m_outputFileBox );

   m_outputFileLabel = new QLabel( tr( "File name:" ) );
   m_outputFileLayout->addWidget( m_outputFileLabel );

   m_outputFileEdit = new QLineEdit( "messages.log" );
   connect( m_outputFileEdit, SIGNAL( textChanged( const QString& ) ),
            this, SLOT( outputFileNameChangedSlot( const QString& ) ) );
   m_outputFileLayout->addWidget( m_outputFileEdit );

   m_outputFileDock = new QDockWidget( tr( "Output file" ), this );
   m_outputFileDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                      Qt::BottomDockWidgetArea );
   m_outputFileDock->setWidget( m_outputFileWidget );

   addDockWidget( Qt::BottomDockWidgetArea, m_outputFileDock );

   return;

}
