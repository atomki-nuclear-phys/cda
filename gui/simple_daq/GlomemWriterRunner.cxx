
// Qt include(s):
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include <QFont>
#include <QIcon>

// CDA include(s):
#include "common/PathResolver.h"

// Local include(s):
#include "GlomemWriterRunner.h"

namespace simple_daq {

   GlomemWriterRunner::GlomemWriterRunner( QWidget* parent,
                                           Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_runner(),
        m_logger( "sd::GlomemWriterRunner" ) {

      setMinimumSize( 300, 150 );
      setMaximumSize( 300, 150 );

      m_mainBox = new QGroupBox( tr( "GloMem writing control" ), this );
      m_mainBox->setGeometry( QRect( 5, 5, 290, 140 ) );

      m_processStatus = new QLabel( tr( "GloMem writer stopped" ), m_mainBox );
      m_processStatus->setGeometry( QRect( 0, 40, 290, 40 ) );
      m_processStatus->setAlignment( Qt::AlignCenter );

      QPalette palette( m_processStatus->palette() );
      palette.setColor( QPalette::Active, QPalette::WindowText,
                        QColor( 10, 150, 10 ) );
      palette.setColor( QPalette::Inactive, QPalette::WindowText,
                        QColor( 10, 150, 10 ) );
      m_processStatus->setPalette( palette );

      QFont font( m_processStatus->font() );
      font.setPointSize( 14 );
      m_processStatus->setFont( font );

      m_starterButton = new QPushButton( QIcon::fromTheme( "media-playback-start" ),
                                         tr( "Start glomem writer" ), m_mainBox );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton, SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      m_runner.setExecName( daq::PathResolver::resolve( "cda-glomem-writer",
                                                        "PATH" ) );
      if( m_runner.getExecName() == "" ) {
         m_logger << msg::WARNING
                  << tr( "Global memory writing not available" )
                  << msg::endmsg;
         m_mainBox->setEnabled( false );
      }
   }

   GlomemWriterRunner::~GlomemWriterRunner() {

      delete m_starterButton;
      delete m_processStatus;
      delete m_mainBox;
   }

   void GlomemWriterRunner::setConfigFileName( const QString& fileName ) {

      m_configFileName = fileName;
      return;
   }

   const QString& GlomemWriterRunner::getConfigFileName() const {

      return m_configFileName;
   }

   void GlomemWriterRunner::setMsgServerAddress( const QString& address ) {

      m_msgServerAddress = address;
      return;
   }

   const QString& GlomemWriterRunner::getMsgServerAddress() const {

      return m_msgServerAddress;
   }

   void GlomemWriterRunner::setEventAddress( const QString& address ) {

      m_eventAddress = address;
      return;
   }

   const QString& GlomemWriterRunner::getEventAddress() const {

      return m_eventAddress;
   }

   void GlomemWriterRunner::setVerbosity( msg::Level verbosity ) {

      m_level = verbosity;
      return;
   }

   msg::Level GlomemWriterRunner::getVerbosity() const {

      return m_level;
   }

   /**
    * @param status The required state for the widget
    */
   void GlomemWriterRunner::setEnabled( bool status ) {

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
   void GlomemWriterRunner::setStatServerAddress( bool status, const QString& address ) {

      if( status ) {
         m_statServerAddresses.insert( address );
      } else {
         m_statServerAddresses.erase( address );
      }
      return;
   }

   void GlomemWriterRunner::startApp( bool start ) {

      if( start ) {

         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );
         options += " -e " + m_eventAddress;

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
            REPORT_ERROR( tr( "Couldn't start GloMem writer!" ) );

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::WindowText,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::WindowText,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Reset" ) );
            m_starterButton->setIcon( QIcon::fromTheme( "edit-clear" ) );

         } else {
            m_logger << msg::INFO << tr( "GloMem writer started" )
                     << msg::endmsg;

            m_processStatus->setText( tr( "GloMem writer running" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::WindowText,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::WindowText,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Stop glomem writer" ) );
            m_starterButton->setIcon( QIcon::fromTheme( "media-playback-stop" ) );

            emit running( true );
            emit receiverRunning( true, m_eventAddress );
         }

      } else {

         if( ! m_runner.stop() ) {
            REPORT_ERROR( tr( "The GloMem writer could not be stopped "
                              "successfully" ) );

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::WindowText,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::WindowText,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

         } else {
            m_logger << msg::INFO << tr( "GloMem writer stopped" )
                     << msg::endmsg;

            m_processStatus->setText( tr( "GloMem writer stopped" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::WindowText,
                              QColor( 10, 150, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::WindowText,
                              QColor( 10, 150, 10 ) );
            m_processStatus->setPalette( palette );
         }

         emit running( false );
         emit receiverRunning( false, m_eventAddress );
         m_starterButton->setText( tr( "Start glomem writer" ) );
         m_starterButton->setIcon( QIcon::fromTheme( "media-playback-start" ) );
      }

      return;
   }

} // namespace simple_daq
