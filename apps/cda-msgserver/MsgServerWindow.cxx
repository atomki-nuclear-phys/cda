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
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QIcon>

// CDA include(s):
#include "msg/Server.h"
#include "msg/View.h"

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
   resize( 750, 450 );
   setWindowTitle( "CDA Message Server" );
   setWindowIcon( QIcon( ":/img/logo.png" ) );

   //
   // Create the objects doing most of the work:
   //
   m_server = new msg::Server();
   m_view   = new msg::View();
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
   // Start listening on port 49700:
   //
   m_server->listen( msg::Address( "127.0.0.1", 49700 ) );

}

/**
 * The destructor only has to delete the two message handling objects,
 * all the graphical objects are taken care of by Qt.
 */
MsgServerWindow::~MsgServerWindow() {

   //
   // Only these two are not taken care of by Qt:
   //
   delete m_view;
   delete m_server;

}

/**
 * When the port changes the server is told to stop listening on the
 * previously configured port and is then re-started to listen on the
 * new port.
 */
void MsgServerWindow::portChangedSlot( int port ) {

   if( m_server->isListening() ) m_server->close();
   m_server->listen( msg::Address( "127.0.0.1", port ) );
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

void MsgServerWindow::aboutQtSlot() {

   QMessageBox::aboutQt( this, "CDA Message Server - built on Qt" );
   return;

}

void MsgServerWindow::aboutCDASlot() {

   QMessageBox::about( this, "CDA - CAMAC Data Acquisition",
                       "CDA is an application suit built for the "
                       "Experimental Nuclear Physics department of the "
                       "Atomki. It is composed of multiple executables "
                       "that work together in the data acquisition." );
   return;

}

void MsgServerWindow::aboutMsgServerSlot() {

   QMessageBox::about( this, "CDA Message Server",
                       "The CDA Message Server is an application in which "
                       "all parts of CDA can be monitored. It listens for "
                       "incoming messages from the various components and "
                       "presents these to the user in a nice way." );
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

   QMenu* fileMenu = menuBar()->addMenu( "File" );

   m_quitAction = fileMenu->addAction( QIcon( ":/img/warning.png" ),
                                       "Quit" );
   connect( m_quitAction, SIGNAL( triggered() ),
            this, SLOT( close() ) );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                Create a "Preferences" menu                  //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* prefMenu = menuBar()->addMenu( "Preferences" );

   m_showPortDock = prefMenu->addAction( "Show port setting" );
   m_showPortDock->setCheckable( true );
   connect( m_showPortDock, SIGNAL( toggled( bool ) ),
            m_portDock, SLOT( setVisible( bool ) ) );
   connect( m_portDock, SIGNAL( visibilityChanged( bool ) ),
            m_showPortDock, SLOT( setChecked( bool ) ) );

   m_showShownLinesDock = prefMenu->addAction( "Show shown lines setting" );
   m_showShownLinesDock->setCheckable( true );
   connect( m_showShownLinesDock, SIGNAL( toggled( bool ) ),
            m_shownLinesDock, SLOT( setVisible( bool ) ) );
   connect( m_shownLinesDock, SIGNAL( visibilityChanged( bool ) ),
            m_showShownLinesDock, SLOT( setChecked( bool ) ) );

   m_showOutputFileDock = prefMenu->addAction( "Show output file setting" );
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
   QMenu* helpMenu = menuBar()->addMenu( "Help" );

   m_aboutQtAction = helpMenu->addAction( "About Qt" );
   connect( m_aboutQtAction, SIGNAL( triggered() ),
            this, SLOT( aboutQtSlot() ) );

   m_aboutMsgServerAction = helpMenu->addAction( "About Message Server" );
   connect( m_aboutMsgServerAction, SIGNAL( triggered() ),
            this, SLOT( aboutMsgServerSlot() ) );

   m_aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                           "About CDA" );
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
   m_portEdit->setMinimum( 10000 );
   m_portEdit->setMaximum( 80000 );
   m_portEdit->setValue( 49700 );
   connect( m_portEdit, SIGNAL( valueChanged( int ) ),
            this, SLOT( portChangedSlot( int ) ) );

   m_portDock = new QDockWidget( "Server port", this );
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
   connect( m_shownLinesEdit, SIGNAL( valueChanged( int ) ),
            m_view, SLOT( setMaximumMessageCount( int ) ) );

   m_shownLinesDock = new QDockWidget( "Max. number of messages", this );
   m_shownLinesDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                      Qt::BottomDockWidgetArea );
   m_shownLinesDock->setWidget( m_shownLinesEdit );

   addDockWidget( Qt::TopDockWidgetArea, m_shownLinesDock );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //       Create the widgets for the output file settings       //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_outputFileBox = new QGroupBox( "Write output file" );
   m_outputFileBox->setCheckable( true );
   connect( m_outputFileBox, SIGNAL( toggled( bool ) ),
            this, SLOT( outputFileWritingChangedSlot( bool ) ) );
   m_outputFileBox->setChecked( false );

   m_outputFileLayout = new QHBoxLayout( m_outputFileBox );

   m_outputFileLabel = new QLabel( "File name: " );
   m_outputFileLayout->addWidget( m_outputFileLabel );

   m_outputFileEdit = new QLineEdit( "messages.log" );
   connect( m_outputFileEdit, SIGNAL( textChanged( const QString& ) ),
            this, SLOT( outputFileNameChangedSlot( const QString& ) ) );
   m_outputFileLayout->addWidget( m_outputFileEdit );

   m_outputFileDock = new QDockWidget( "Output file", this );
   m_outputFileDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                      Qt::BottomDockWidgetArea );
   m_outputFileDock->setWidget( m_outputFileBox );

   addDockWidget( Qt::BottomDockWidgetArea, m_outputFileDock );

   return;

}
