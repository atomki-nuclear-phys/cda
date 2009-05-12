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
#include "GlomemWriterRunner.h"

namespace simple_daq {

   GlomemWriterRunner::GlomemWriterRunner( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_msgServerAddress( "127.0.0.1:50000" ),
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
      palette.setColor( QPalette::Active, QPalette::Foreground,
                        QColor( 10, 150, 10 ) );
      palette.setColor( QPalette::Inactive, QPalette::Foreground,
                        QColor( 10, 150, 10 ) );
      m_processStatus->setPalette( palette );

      QFont font( m_processStatus->font() );
      font.setPointSize( 14 );
      m_processStatus->setFont( font );

      m_starterButton = new QPushButton( tr( "Start glomem writer" ), m_mainBox );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton, SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      m_runner.setExecName( daq::PathResolver::resolve( "cda-glomem-writer",
                                                        "PATH" ) );

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

   void GlomemWriterRunner::startApp( bool start ) {

      if( start ) {

         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );
         options += " -e " + m_eventAddress;

         m_logger << msg::DEBUG << "Using options: " << options << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            m_logger << msg::ERROR << "Couldn't start GloMem writer!" << msg::endmsg;

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Reset" ) );

         } else {
            m_logger << msg::INFO << "GloMem writer started" << msg::endmsg;

            m_processStatus->setText( tr( "GloMem writer running" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Stop glomem writer" ) );

            emit running( true );

         }

      } else {

         if( ! m_runner.stop() ) {
            m_logger << msg::ERROR << "The GloMem writer could not be stopped "
                     << "successfully" << msg::endmsg;

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

         } else {
            m_logger << msg::INFO << "GloMem writer stopped" << msg::endmsg;

            m_processStatus->setText( tr( "GloMem writer stopped" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 10, 150, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Start glomem writer" ) );

            emit running( false );

         }

      }

      return;

   }

} // namespace simple_daq
