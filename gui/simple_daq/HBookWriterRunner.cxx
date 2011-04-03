// $Id$

// Qt include(s):
#include <QtCore/QTimer>
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QFont>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdadaq/common/PathResolver.h"
#else
#   include "common/PathResolver.h"
#endif

// Local include(s):
#include "HBookWriterRunner.h"

namespace simple_daq {

   HBookWriterRunner::HBookWriterRunner( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_msgServerAddress( "127.0.0.1:50000" ),
        m_runner(),
        m_logger( "sd::HBookWriterRunner" ) {

      setMinimumSize( 300, 150 );
      setMaximumSize( 300, 150 );

      m_mainBox = new QGroupBox( tr( "HBook writing control" ), this );
      m_mainBox->setGeometry( QRect( 5, 5, 290, 140 ) );

      //
      // Set up the big label showing the application status:
      //
      m_processStatus = new QLabel( tr( "HBook writer stopped" ), m_mainBox );
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
      // Set up the widgets for the automatic file name updates:
      //
      m_updateFrequencyLabel = new QLabel( tr( "Update freq. [min]:" ), m_mainBox );
      m_updateFrequencyLabel->setGeometry( QRect( 10, 52, 140, 20 ) );
      m_updateFrequencyLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_updateFrequency = new QSpinBox( m_mainBox );
      m_updateFrequency->setGeometry( QRect( 160, 50, 100, 24 ) );
      m_updateFrequency->setMinimum( 1 );
      m_updateFrequency->setMaximum( 360 );
      m_updateFrequency->setValue( 120 );

      //
      // Set up the widgets for the file name specification:
      //
      m_fileNameLabel = new QLabel( tr( "Output file:" ), m_mainBox );
      m_fileNameLabel->setGeometry( QRect( 0, 75, 100, 25 ) );
      m_fileNameLabel->setAlignment( Qt::AlignCenter );

      m_fileNameEdit = new QLineEdit( m_mainBox );
      m_fileNameEdit->setGeometry( QRect( 100, 75, 170, 25 ) );

      //
      // Set up the button starting the application:
      //
      m_starterButton = new QPushButton( tr( "Start hbook writer" ), m_mainBox );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton, SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      m_runner.setExecName( daq::PathResolver::resolve( "cda-hbook-writer",
                                                        "PATH" ) );

      //
      // Set up the timer updating the file name in regular intervals:
      //
      m_updateTimer = new QTimer( this );
      m_updateTimer->setSingleShot( false );
      connect( m_updateTimer, SIGNAL( timeout() ),
               this, SLOT( updateFileName() ) );

   }

   void HBookWriterRunner::setConfigFileName( const QString& fileName ) {

      m_configFileName = fileName;
      return;
   }

   const QString& HBookWriterRunner::getConfigFileName() const {

      return m_configFileName;
   }

   void HBookWriterRunner::setMsgServerAddress( const QString& address ) {

      m_msgServerAddress = address;
      return;
   }

   const QString& HBookWriterRunner::getMsgServerAddress() const {

      return m_msgServerAddress;
   }

   void HBookWriterRunner::setEventAddress( const QString& address ) {

      m_eventAddress = address;
      return;
   }

   const QString& HBookWriterRunner::getEventAddress() const {

      return m_eventAddress;
   }

   void HBookWriterRunner::setVerbosity( msg::Level verbosity ) {

      m_level = verbosity;
      return;
   }

   msg::Level HBookWriterRunner::getVerbosity() const {

      return m_level;
   }

   void HBookWriterRunner::startApp( bool start ) {

      if( start ) {

         //
         // Set up some things for the automatic file name updating:
         //
         m_fileNameEdit->setReadOnly( true );
         // The interval has to be specified in miliseconds, hence the 60000 multiplier:
         m_updateTimer->setInterval( m_updateFrequency->value() * 60000 );
         m_updateFrequency->setEnabled( false );

         //
         // Construct the file name:
         //
         QString fileName = m_fileNameEdit->text();
         bool updateFileName = false;
         if( fileName.contains( "%1" ) ) {
            m_logger << msg::INFO << tr( "File name will be updated every %1 minutes" )
               .arg( m_updateFrequency->value() ) << msg::endmsg;
            m_currentFileNumber = 1;
            fileName = fileName.arg( m_currentFileNumber, ( int ) 3, ( int ) 10, QLatin1Char( '0' ) );
            updateFileName = true;
         }

         //
         // Construct the application arguments:
         //
         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );
         options += " -e " + m_eventAddress;
         options += " -o " + fileName;

         m_logger << msg::DEBUG << tr( "Using options: %1" ).arg( options )
                  << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            m_logger << msg::ERROR << tr( "Couldn't start HBook writer!" )
                     << msg::endmsg;

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Reset" ) );

         } else {
            m_logger << msg::INFO << tr( "HBook writer started" )
                     << msg::endmsg;

            m_processStatus->setText( tr( "HBook writer running" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Stop hbook writer" ) );

            if( updateFileName ) m_updateTimer->start();

            emit running( true );

         }

      } else {

         m_updateTimer->stop();
         m_updateFrequency->setEnabled( true );

         if( ! m_runner.stop() ) {
            m_logger << msg::ERROR
                     << tr( "The HBook writer could not be stopped "
                            "successfully" ) << msg::endmsg;

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

         } else {
            m_logger << msg::INFO << tr( "HBook writer stopped" ) << msg::endmsg;

            m_processStatus->setText( tr( "HBook writer stopped" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Start hbook writer" ) );

            m_fileNameEdit->setReadOnly( false );

            emit running( false );

         }

      }

      return;
   }

   void HBookWriterRunner::updateFileName() {

      // Let everybody know that we're not running anymore:
      emit running( false );

      //
      // Now, stop the currently running process:
      //
      if( ! m_runner.stop() ) {
         m_logger << msg::ERROR << tr( "Couldn't stop cda-hbook-writer as part of the file name update!" )
                  << msg::endmsg;
         m_processStatus->setText( tr( "ERROR" ) );
         QPalette palette( m_processStatus->palette() );
         palette.setColor( QPalette::Active, QPalette::Foreground,
                           QColor( 150, 10, 10 ) );
         palette.setColor( QPalette::Inactive, QPalette::Foreground,
                           QColor( 150, 10, 10 ) );
         m_processStatus->setPalette( palette );
         return;
      }

      //
      // Construct the file name:
      //
      QString fileName = m_fileNameEdit->text();
      if( ! ( ( fileName.count( "\%" ) == 1 ) &&
              ( fileName.count( "\%1" ) == 1 ) ) ) {
         m_logger << msg::ERROR << tr( "File name not in the correct format! Put a \"%1\" where the "
                                       "file number should be!" ) << msg::endmsg;
         return;
      }
      ++m_currentFileNumber;
      fileName = fileName.arg( m_currentFileNumber, ( int ) 3, ( int ) 10, QLatin1Char( '0' ) );

      //
      // Construct the application arguments:
      //
      QString options;
      options += " -m " + m_msgServerAddress;
      options += " -c " + m_configFileName;
      options += " -v " + QString::number( m_level );
      options += " -e " + m_eventAddress;
      options += " -o " + fileName;

      m_logger << msg::DEBUG << tr( "Using options: %1" ).arg( options ) << msg::endmsg;
      m_runner.setOptions( options );

      //
      // Now restart the application:
      //
      if( ! m_runner.start() ) {
         m_logger << msg::ERROR << tr( "Couldn't restart cda-hbook-writer as part of the file name update!" )
                  << msg::endmsg;
         m_processStatus->setText( tr( "ERROR" ) );
         QPalette palette( m_processStatus->palette() );
         palette.setColor( QPalette::Active, QPalette::Foreground,
                           QColor( 150, 10, 10 ) );
         palette.setColor( QPalette::Inactive, QPalette::Foreground,
                           QColor( 150, 10, 10 ) );
         m_processStatus->setPalette( palette );
         return;
      }

      //
      // Let's wait a moment before telling others that we are running again:
      //
      QTimer::singleShot( 1000, this, SLOT( emitRunningTrue() ) );

      return;
   }

   void HBookWriterRunner::emitRunningTrue() {

      // Let everybody know that we're running again:
      emit running( true );

      return;
   }

} // namespace simple_daq
