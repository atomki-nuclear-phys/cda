// $Id$

// Qt include(s):
#include <QtCore/QTime>
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

      //
      // Create the label showing if the application is running:
      //
      m_processStatus = new QLabel( tr( "Camac reader stopped" ), m_mainBox );
      m_processStatus->setGeometry( QRect( 0, 15, 290, 40 ) );
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
      // Draw the "Start time" label:
      //
      m_startTimeLabel = new QLabel( tr( "Start time:" ), m_mainBox );
      m_startTimeLabel->setGeometry( QRect( 10, 55, 140, 20 ) );
      m_startTimeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_startTime = new QLabel( tr( "N/A" ), m_mainBox );
      m_startTime->setGeometry( QRect( 160, 55, 100, 20 ) );
      m_startTime->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      //
      // Draw the "Stop time" label:
      //
      m_stopTimeLabel = new QLabel( tr( "Stop time:" ), m_mainBox );
      m_stopTimeLabel->setGeometry( QRect( 10, 75, 140, 20 ) );
      m_stopTimeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_stopTime = new QLabel( tr( "N/A" ), m_mainBox );
      m_stopTime->setGeometry( QRect( 160, 75, 100, 20 ) );
      m_stopTime->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

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
      m_hbookWriterUpdating = false;
      m_hbookWriterRunning = false;
      m_glomemWriterRunning = false;

   }

   CamacReaderRunner::~CamacReaderRunner() {

      delete m_stopTimeLabel; delete m_stopTime;
      delete m_startTimeLabel; delete m_startTime;
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
    * @param address The statistics server address to be used by the application
    */
   void CamacReaderRunner::setStatServerAddress( const QString& address ) {

      m_statServerAddress = address;
      return;
   }

   /**
    * @returns The statistics server address to be used by the application
    */
   const QString& CamacReaderRunner::getStatServerAddress() const {

      return m_statServerAddress;
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

      // Update the internal flag:
      m_hbookWriterRunning = running;

      //
      // Halt the CAMAC reader as long as the HBOOK writer is updating its file name:
      //
      if( ( ! running ) && ( m_runner.isRunning() ) ) {
         if( ! m_runner.stop() ) {
            m_logger << msg::ERROR << tr( "Couldn't halt cda-camac-reader for the duration of the HBOOK "
                                          "file name change!" ) << msg::endmsg;
            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );
            return;
         } else {
            m_logger << msg::INFO << tr( "Temporarily halted cda-camac-reader" ) << msg::endmsg;
         }
         m_hbookWriterUpdating = true;
      }

      //
      // Restart the CAMAC reader when the file name updating finished:
      //
      if( running && ( ! m_runner.isRunning() ) && m_hbookWriterUpdating ) {
         m_hbookWriterUpdating = false;
         if( ! m_runner.start() ) {
            m_logger << msg::ERROR << tr( "Couldn't restart cda-camac-reader for the duration of the HBOOK "
                                          "file name change!" ) << msg::endmsg;
            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );
            return;
         } else {
            m_logger << msg::INFO << tr( "Re-started cda-camac-reader" ) << msg::endmsg;
         }
      }

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

         if( ! m_statServerAddress.isEmpty() ) {
            options += " -s " + m_statServerAddress;
         }

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
         m_logger << msg::DEBUG << tr( "Using options: %1" ).arg( options ) << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            m_logger << msg::ERROR << tr( "Couldn't start camac reader!" ) << msg::endmsg;

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
            m_logger << msg::INFO << tr( "Camac reader started" ) << msg::endmsg;

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

            // Set up the time label(s):
            m_startTime->setText( QTime::currentTime().toString() );
            m_stopTime->setText( tr( "N/A" ) );

            m_starterButton->setText( tr( "Stop camac reader" ) );

         }

      } else {

         if( ! m_runner.stop() ) {
            m_logger << msg::ERROR << tr( "The camac reader could not be stopped "
                                          "successfully" ) << msg::endmsg;

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
            m_logger << msg::INFO << tr( "Camac reader stopped" ) << msg::endmsg;

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

            // Set up the time label(s):
            m_stopTime->setText( QTime::currentTime().toString() );

            m_starterButton->setText( tr( "Start camac reader" ) );
         }

      }

      return;
   }

} // namespace simple_daq
