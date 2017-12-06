
// Qt include(s):
#include <QTime>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include <QFont>
#include <QIcon>

// CDA include(s):
#include "common/PathResolver.h"

// Local include(s):
#include "CaenVmeReaderRunner.h"

namespace simple_daq {

   CaenVmeReaderRunner::CaenVmeReaderRunner( QWidget* parent,
                                             Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_runner(),
        m_logger( "sd::CaenVmeReaderRunner" ) {

      //
      // Fix the size of the widget:
      //
      setMinimumSize( 300, 150 );
      setMaximumSize( 300, 150 );

      //
      // Create the main group box for all the other widgets:
      //
      m_mainBox.reset( new QGroupBox( tr( "VME acquisition control" ),
                                      this ) );
      m_mainBox->setGeometry( QRect( 5, 5, 290, 140 ) );

      //
      // Create the label showing if the application is running:
      //
      m_processStatus.reset( new QLabel( tr( "VME reader stopped" ),
                                         m_mainBox.get() ) );
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
      m_startTimeLabel.reset( new QLabel( tr( "Start time:" ),
                                          m_mainBox.get() ) );
      m_startTimeLabel->setGeometry( QRect( 10, 55, 140, 20 ) );
      m_startTimeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_startTime.reset( new QLabel( tr( "N/A" ), m_mainBox.get() ) );
      m_startTime->setGeometry( QRect( 160, 55, 100, 20 ) );
      m_startTime->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      //
      // Draw the "Stop time" label:
      //
      m_stopTimeLabel.reset( new QLabel( tr( "Stop time:" ),
                                         m_mainBox.get() ) );
      m_stopTimeLabel->setGeometry( QRect( 10, 75, 140, 20 ) );
      m_stopTimeLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_stopTime.reset( new QLabel( tr( "N/A" ), m_mainBox.get() ) );
      m_stopTime->setGeometry( QRect( 160, 75, 100, 20 ) );
      m_stopTime->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      //
      // Create the button starting and stopping the application:
      //
      m_starterButton.reset(
               new QPushButton( QIcon::fromTheme( "media-playback-start" ),
                                tr( "Start VME reader" ),
                                m_mainBox.get() ) );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton.get(), SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      //
      // Configure the application runner object:
      //
      m_runner.setExecName( daq::PathResolver::resolve( "cda-caen-vme-reader",
                                                        "PATH" ) );
      if( m_runner.getExecName() == "" ) {
         REPORT_ERROR( tr( "cda-caen-vme-reader not found. Data acquisition "
                           "is not possible." ) );
         m_mainBox->setEnabled( false );
      }
   }

   /**
    * @param fileName The name of the configuration file to be used
    */
   void CaenVmeReaderRunner::setConfigFileName( const QString& fileName ) {

      m_configFileName = fileName;
      return;
   }

   /**
    * @returns The name of the configuration file to be used
    */
   const QString& CaenVmeReaderRunner::getConfigFileName() const {

      return m_configFileName;
   }

   /**
    * @param address The message server address to be used by the application
    */
   void CaenVmeReaderRunner::setMsgServerAddress( const QString& address ) {

      m_msgServerAddress = address;
      return;
   }

   /**
    * @returns The message server address to be used by the application
    */
   const QString& CaenVmeReaderRunner::getMsgServerAddress() const {

      return m_msgServerAddress;
   }

   /**
    * @param verbosity The output verbosity of cda-caen-reader
    */
   void CaenVmeReaderRunner::setVerbosity( msg::Level verbosity ) {

      m_level = verbosity;
      return;
   }

   /**
    * @returns The output verbosity of cda-caen-reader
    */
   msg::Level CaenVmeReaderRunner::getVerbosity() const {

      return m_level;
   }

   /**
    * @param status The required state for the widget
    */
   void CaenVmeReaderRunner::setEnabled( bool status ) {

      // Always allow disabling the widgets:
      if( ! status ) {
         m_mainBox->setEnabled( status );
      }
      // Only enable the widgets if cda-caen-reader has been found:
      else if( m_runner.getExecName() != "" ) {
         m_mainBox->setEnabled( status );
      }

      return;
   }

   void CaenVmeReaderRunner::setWriterRunning( bool running,
                                               const QString& address ) {

      // Update the running applications flag:
      if( running ) {
         m_eventListenerAddresses.insert( address );
      } else {
         m_eventListenerAddresses.erase( address );
      }

      return;
   }

   /**
    * @param status Selects whether the address should be added or removed
    * @param address The statistics server address to be used by the application
    */
   void CaenVmeReaderRunner::setStatServerAddress( bool status,
                                                   const QString& address ) {

      if( status ) {
         m_statServerAddresses.insert( address );
      } else {
         m_statServerAddresses.erase( address );
      }
      return;
   }

   /**
    * This is the function doing most of the work in this class. It puts the
    * command line options together to start cda-caen-reader correctly, then
    * it starts the application in a new process.
    *
    * @param start <code>true</code> if the application should be started,
    *              <code>false</code> if it should be stopped
    */
   void CaenVmeReaderRunner::startApp( bool start ) {

      if( start ) {

         //
         // Collect the trivial command line options:
         //
         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );

         //
         // Collect where the application should send statistics information to:
         //
         for( const QString& server : m_statServerAddresses ) {
            options += " -s " + server;
         }

         //
         // Collect where the application should send events to:
         //
         for( const QString& server : m_eventListenerAddresses ) {
            options += " -e " + server;
         }

         //
         // Set these options to the runner object:
         //
         m_logger << msg::DEBUG << tr( "Using options: %1" ).arg( options )
                  << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            REPORT_ERROR( tr( "Couldn't start CAEN VME reader!" ) );

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
            m_starterButton->setIcon( QIcon::fromTheme( "edit-clear" ) );

         } else {
            m_logger << msg::INFO << tr( "CAEN VME reader started" )
                     << msg::endmsg;

            //
            // Signal that the application is running:
            //
            m_processStatus->setText( tr( "VME reader running" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            // Set up the time label(s):
            m_startTime->setText( QTime::currentTime().toString() );
            m_stopTime->setText( tr( "N/A" ) );

            m_starterButton->setText( tr( "Stop VME reader" ) );
            m_starterButton->setIcon(
                     QIcon::fromTheme( "media-playback-stop" ) );

         }

      } else {

         if( ! m_runner.stop() ) {
            REPORT_ERROR( tr( "The CAEN VME reader could not be stopped "
                              "successfully" ) );

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
            m_logger << msg::INFO << tr( "CAEN VME reader stopped" )
                     << msg::endmsg;

            //
            // Return the widgets to their "normal" state:
            //
            m_processStatus->setText( tr( "VME reader stopped" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            m_processStatus->setPalette( palette );

            // Set up the time label(s):
            m_stopTime->setText( QTime::currentTime().toString() );
         }

         m_starterButton->setText( tr( "Start VME reader" ) );
         m_starterButton->setIcon( QIcon::fromTheme( "media-playback-start" ) );

      }

      return;
   }

} // namespace simple_daq
