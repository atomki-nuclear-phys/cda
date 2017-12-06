
// Qt include(s):
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include <QFont>
#include <QLineEdit>
#include <QSpinBox>
#include <QIcon>

// CDA include(s):
#include "common/PathResolver.h"

// Local include(s):
#include "HBookWriterRunner.h"

namespace simple_daq {

   HBookWriterRunner::HBookWriterRunner( QWidget* parent,
                                         Qt::WindowFlags flags )
      : QWidget( parent, flags ),
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
      m_updateFrequency->setToolTip( tr( "Set the intervals at which a new output "
                                         "file should be opened" ) );

      //
      // Set up the widgets for the file name specification:
      //
      m_fileNameLabel = new QLabel( tr( "Output file:" ), m_mainBox );
      m_fileNameLabel->setGeometry( QRect( 0, 75, 100, 25 ) );
      m_fileNameLabel->setAlignment( Qt::AlignCenter );

      m_fileNameEdit = new QLineEdit( m_mainBox );
      m_fileNameEdit->setGeometry( QRect( 100, 75, 170, 25 ) );
      m_fileNameEdit->setToolTip( tr( "To activate automatic file name updating, put\n"
                                      "\"%1\" in the file name where a number should\n"
                                      "be filled in. To disable the automatic updating\n"
                                      "just don't put this symbol in the file name." ) );

      //
      // Set up the button starting the application:
      //
      m_starterButton = new QPushButton( QIcon::fromTheme( "media-playback-start" ),
                                         tr( "Start hbook writer" ), m_mainBox );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton, SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      m_runner.setExecName( daq::PathResolver::resolve( "cda-hbook-writer",
                                                        "PATH" ) );
      if( m_runner.getExecName() == "" ) {
         m_logger << msg::WARNING
                  << tr( "HBOOK file writing not available" )
                  << msg::endmsg;
         m_mainBox->setEnabled( false );
      }
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

   /**
    * @param status The required state for the widget
    */
   void HBookWriterRunner::setEnabled( bool status ) {

      // Always allow disabling the widgets:
      if( ! status ) {
         m_mainBox->setEnabled( status );
      }
      // Only enable the widgets if cda-glomem-writer has been found:
      else if( m_runner.getExecName() != "" ) {
         m_mainBox->setEnabled( status );
      }

      return;
   }

   /**
    * @param status Selects whether the address should be added or removed
    * @param address The statistics server address to be used by the application
    */
   void HBookWriterRunner::setStatServerAddress( bool status, const QString& address ) {

      if( status ) {
         m_statServerAddresses.insert( address );
      } else {
         m_statServerAddresses.erase( address );
      }
      return;
   }

   void HBookWriterRunner::startApp( bool start ) {

      if( start ) {

         //
         // Set up some things for the automatic file name updating:
         //
         m_fileNameEdit->setReadOnly( true );
         m_updateFrequency->setEnabled( false );


         //
         // Construct the application arguments:
         //
         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );
         options += " -e " + m_eventAddress;
         options += " -o " + m_fileNameEdit->text();
         options += " -u " + QString::number( m_updateFrequency->value() );

         //
         // Collect where the application should send statistics information to:
         //
         for( const QString& server : m_statServerAddresses ) {
            options += " -s " + server;
         }

         m_logger << msg::DEBUG << tr( "Using options: %1" ).arg( options )
                  << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            REPORT_ERROR( tr( "Couldn't start HBook writer!" ) );

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
            m_starterButton->setIcon( QIcon::fromTheme( "media-playback-stop" ) );

            emit running( true );
            emit receiverRunning( true, m_eventAddress );
         }

      } else {

         if( ! m_runner.stop() ) {
            REPORT_ERROR( tr( "The HBook writer could not be stopped "
                              "successfully" ) );

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
         }

         emit running( false );
         emit receiverRunning( false, m_eventAddress );
         m_starterButton->setText( tr( "Start hbook writer" ) );
         m_starterButton->setIcon( QIcon::fromTheme( "media-playback-start" ) );

         m_fileNameEdit->setReadOnly( false );
         m_updateFrequency->setEnabled( true );
      }

      return;
   }

} // namespace simple_daq
