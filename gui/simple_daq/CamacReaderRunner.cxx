// $Id$

// Qt include(s):
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QFont>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdadaq/common/PathResolver.h"
#else
#   include "common/PathResolver.h"
#endif

// Local include(s):
#include "CamacReaderRunner.h"

namespace simple_daq {

   CamacReaderRunner::CamacReaderRunner( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_msgServerAddress( "127.0.0.1:50000" ),
        m_runner(),
        m_logger( "sd::CamacReaderRunner" ) {

      //
      // Fix the size of the widget:
      //
      setMinimumSize( 300, 150 );
      setMaximumSize( 300, 150 );

      //
      // Create the main group box for all the other widgets:
      //
      m_mainBox = new QGroupBox( tr( "Camac acquisition control" ), this );
      m_mainBox->setGeometry( QRect( 5, 5, 290, 140 ) );
      m_mainBox->setFlat( true );
      m_mainBox->setAlignment( Qt::AlignHCenter );

      //
      // Create the label showing if the application is running:
      //
      m_processStatus = new QLabel( tr( "Camac reader stopped" ), m_mainBox );
      m_processStatus->setGeometry( QRect( 0, 40, 290, 40 ) );
      m_processStatus->setAlignment( Qt::AlignCenter );

      QPalette palette( m_processStatus->palette() );
      palette.setColor( QPalette::Active, QPalette::Foreground,
                        QColor( 10, 150, 10 ) );
      palette.setColor( QPalette::Inactive, QPalette::Foreground,
                        QColor( 10, 150, 10 ) );
      m_processStatus->setPalette( palette );

      QFont font( m_processStatus->font() );
      font.setPointSize( 14 );
      m_processStatus->setFont( font );

      //
      // Create the button starting and stopping the application:
      //
      m_starterButton = new QPushButton( tr( "Start camac reader" ), m_mainBox );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton, SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      //
      // Configure the application runner object:
      //
      m_runner.setExecName( daq::PathResolver::resolve( "cda-camac-reader",
                                                        "PATH" ) );

      //
      // Reset the internal flags:
      //
      m_hbookWriterRunning = false;
      m_glomemWriterRunning = false;

   }

   CamacReaderRunner::~CamacReaderRunner() {

      delete m_processStatus;
      delete m_starterButton;
      delete m_mainBox;

   }

   /**
    * @param fileName The name of the configuration file to be used
    */
   void CamacReaderRunner::setConfigFileName( const QString& fileName ) {

      m_configFileName = fileName;
      return;

   }

   /**
    * @returns The name of the configuration file to be used
    */
   const QString& CamacReaderRunner::getConfigFileName() const {

      return m_configFileName;

   }

   /**
    * @param address The message server address to be used by the application
    */
   void CamacReaderRunner::setMsgServerAddress( const QString& address ) {

      m_msgServerAddress = address;
      return;

   }

   /**
    * @returns The message server address to be used by the application
    */
   const QString& CamacReaderRunner::getMsgServerAddress() const {

      return m_msgServerAddress;

   }

   /**
    * @param address The address where cda-hbook-writer waits for events
    */
   void CamacReaderRunner::setHBookWriterAddress( const QString& address ) {

      m_hbookWriterAddress = address;
      return;

   }

   /**
    * @returns The address where cda-hbook-writer waits for events
    */
   const QString& CamacReaderRunner::getHBookWriterAddress() const {

      return m_hbookWriterAddress;

   }

   /**
    * @param address The address where cda-glomem-writer waits for events
    */
   void CamacReaderRunner::setGlomemWriterAddress( const QString& address ) {

      m_glomemWriterAddress = address;
      return;

   }

   /**
    * @returns The address where cda-glomem-writer waits for events
    */
   const QString& CamacReaderRunner::getGlomemWriterAddress() const {

      return m_glomemWriterAddress;

   }

   /**
    * @param verbosity The output verbosity of cda-camac-reader
    */
   void CamacReaderRunner::setVerbosity( msg::Level verbosity ) {

      m_level = verbosity;
      return;

   }

   /**
    * @returns The output verbosity of cda-camac-reader
    */
   msg::Level CamacReaderRunner::getVerbosity() const {

      return m_level;

   }

   void CamacReaderRunner::setHBookWriterRunning( bool running ) {

      m_hbookWriterRunning = running;
      return;

   }

   void CamacReaderRunner::setGlomemWriterRunning( bool running ) {

      m_glomemWriterRunning = running;
      return;

   }

   /**
    * This is the function doing most of the work in this class. It puts the
    * command line options together to start cda-camac-reader correctly, then
    * it starts the application in a new process.
    *
    * @param start <code>true</code> if the application should be started,
    *              <code>false</code> if it should be stopped
    */
   void CamacReaderRunner::startApp( bool start ) {

      if( start ) {

         //
         // Collect the trivial command line options:
         //
         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );

         //
         // Collect where the application should send events to:
         //
         if( m_hbookWriterRunning || m_glomemWriterRunning ) {
            options += " -e ";
            if( m_hbookWriterRunning ) {
               options += m_hbookWriterAddress + " ";
            }
            if( m_glomemWriterRunning ) {
               options += m_glomemWriterAddress;
            }
         }

         //
         // Set these options to the runner object:
         //
         m_logger << msg::DEBUG << "Using options: " << options << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            m_logger << msg::ERROR << "Couldn't start camac reader!" << msg::endmsg;

            //
            // Signal the error as much as possible:
            //
            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Reset" ) );

         } else {
            m_logger << msg::INFO << "Camac reader started" << msg::endmsg;

            //
            // Signal that the application is running:
            //
            m_processStatus->setText( tr( "Camac reader running" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Stop camac reader" ) );

         }

      } else {

         if( ! m_runner.stop() ) {
            m_logger << msg::ERROR << "The camac reader could not be stopped "
                     << "successfully" << msg::endmsg;

            //
            // Signal the error as much as possible:
            //
            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

         } else {
            m_logger << msg::INFO << "Camac reader stopped" << msg::endmsg;

            //
            // Return the widgets to their "normal" state:
            //
            m_processStatus->setText( tr( "Camac reader stopped" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Start camac reader" ) );

         }

      }

      return;

   }

} // namespace simple_daq
