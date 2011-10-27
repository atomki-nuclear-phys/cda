// $Id$

// Qt include(s):
#include <QtGui/QSpinBox>
#include <QtGui/QDockWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QStatusBar>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QIcon>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdadaq/stat/Server.h"
#   include "cdagui/stat/MDIView.h"
#   include "cdagui/common/aboutCDA.h"
#else
#   include "common/Address.h"
#   include "stat/Server.h"
#   include "stat/MDIView.h"
#   include "common/aboutCDA.h"
#endif

// Local include(s):
#include "StatServerWindow.h"

/**
 * The constructor creates the layout of the window, and connects up the various
 * objects to interact with each other.
 */
StatServerWindow::StatServerWindow()
   : QMainWindow() {

   //
   // Set up the window. Notice, that I assume that the CDA logo is
   // embedded in the executable in the "/img"
   // "virtual directory".
   //
   resize( 750, 500 );
   setWindowTitle( tr( "CDA Statistics Server" ) );
   setWindowIcon( QIcon( ":/img/cda-stat-server.png" ) );

   //
   // Create the objects doing most of the work:
   //
   m_server = new stat::Server( this );
   m_view = new stat::MDIView();
   setCentralWidget( m_view );

   //
   // Connect up these two objects:
   //
   connect( m_server, SIGNAL( statAvailable( const stat::Statistics& ) ),
            m_view, SLOT( handleStatistics( const stat::Statistics& ) ) );

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
   // Start listening on port 35000 with all network interfaces:
   //
   if( ! m_server->listen( Address( "0.0.0.0", 35000 ) ) ) {
      QMessageBox::critical( this, tr( "Error starting server" ),
                             tr( "The TCP/IP statistics server could not be started "
                                 "on port %1. Please select another port!" ).arg( 35000 ) );
      statusBar()->showMessage( tr( "The server is stopped" ) );
   } else {
      statusBar()->showMessage( tr( "The server is running on port %1" ).arg( 35000 ) );
   }

}

/**
 * The destructor only has to delete the two statistics handling objects,
 * all the graphical objects are taken care of by Qt.
 */
StatServerWindow::~StatServerWindow() {

   //
   // Only these two are not taken care of by Qt:
   //
   delete m_server;
   delete m_view;

}

void StatServerWindow::aboutQtSlot() {

   QMessageBox::aboutQt( this, tr( "CDA Statistics Server - built on Qt" ) );
   return;

}

void StatServerWindow::aboutStatServerSlot() {

   QMessageBox::about( this, tr( "CDA Statistics Server" ),
                       tr( "The CDA Statistics Server can be used to monitor a "
                           "\"full-blown\" CDA DAQ system, composed of applications "
                           "running on multiple computers. The status of the event "
                           "processing in each of these applications can be monitored "
                           "in this application." ) );
   return;

}

void StatServerWindow::aboutCDASlot() {

   aboutCDA( this );
   return;

}

void StatServerWindow::portChangedSlot() {

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
                             tr( "The TCP/IP statistics server could not be "
                                 "started on port %1. Please select another "
                                 "port!" ).arg( port ) );
      statusBar()->showMessage( tr( "The server is stopped" ) );
   } else {
      statusBar()->showMessage( tr( "The server is running on port %1" ).arg( port ) );
   }
   return;

}

/**
 * This is where the menus are set up. Notice that the function assumes that
 * the dock widgets have already been set up, so this function has to be
 * called after createDockWidgets() has already run.
 */
void StatServerWindow::createMenus() {

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                    Create a "File" menu                     //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* fileMenu = menuBar()->addMenu( tr( "&File" ) );

   QAction* quitAction = fileMenu->addAction( QIcon( ":/img/warning.png" ),
                                              tr( "&Quit" ) );
   connect( quitAction, SIGNAL( triggered() ),
            this, SLOT( close() ) );

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                Create a "Preferences" menu                  //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   QMenu* prefMenu = menuBar()->addMenu( tr( "&Preferences" ) );

   m_showPortDock = prefMenu->addAction( tr( "Show port setting" ) );
   m_showPortDock->setCheckable( true );
   connect( m_showPortDock, SIGNAL( toggled( bool ) ),
            m_portDock, SLOT( setVisible( bool ) ) );
   connect( m_portDock, SIGNAL( visibilityChanged( bool ) ),
            m_showPortDock, SLOT( setChecked( bool ) ) );


   /////////////////////////////////////////////////////////////////
   //                                                             //
   //                    Create a "Help" menu                     //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   menuBar()->addSeparator();
   QMenu* helpMenu = menuBar()->addMenu( tr( "&Help" ) );

   QAction* aboutQtAction = helpMenu->addAction( tr( "About &Qt" ) );
   connect( aboutQtAction, SIGNAL( triggered() ),
            this, SLOT( aboutQtSlot() ) );

   QAction* aboutStatServerAction = helpMenu->addAction( tr( "&About Statistics Server" ) );
   connect( aboutStatServerAction, SIGNAL( triggered() ),
            this, SLOT( aboutStatServerSlot() ) );

   QAction* aboutCDAAction = helpMenu->addAction( QIcon( ":/img/logo.png" ),
                                                  tr( "About &CDA" ) );
   connect( aboutCDAAction, SIGNAL( triggered() ),
            this, SLOT( aboutCDASlot() ) );

   return;

}

/**
 * Create the dock widget(s) handling all the configurations of the
 * statistics server and viewer. This function has to be called early
 * in the initialisation of the window, but after the statistics server
 * and statistics viewer objects are already created.
 */
void StatServerWindow::createDockWidgets() {

   /////////////////////////////////////////////////////////////////
   //                                                             //
   //              Create the port editing widgets                //
   //                                                             //
   /////////////////////////////////////////////////////////////////

   m_portEdit = new QSpinBox();
   m_portEdit->setMinimum( 100 );
   m_portEdit->setMaximum( 80000 );
   m_portEdit->setValue( 35000 );
   connect( m_portEdit, SIGNAL( editingFinished() ),
            this, SLOT( portChangedSlot() ) );

   m_portDock = new QDockWidget( tr( "Server port" ), this );
   m_portDock->setAllowedAreas( Qt::TopDockWidgetArea |
                                Qt::BottomDockWidgetArea );
   m_portDock->setWidget( m_portEdit );

   addDockWidget( Qt::TopDockWidgetArea, m_portDock );

   return;

}
