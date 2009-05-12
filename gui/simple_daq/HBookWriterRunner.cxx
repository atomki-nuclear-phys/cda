// $Id$

// Qt include(s):
#include <QtGui/QGroupBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QPalette>
#include <QtGui/QFont>
#include <QtGui/QLineEdit>

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

      m_processStatus = new QLabel( tr( "HBook writer stopped" ), m_mainBox );
      m_processStatus->setGeometry( QRect( 0, 30, 290, 40 ) );
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

      m_fileNameLabel = new QLabel( tr( "Output file:" ), m_mainBox );
      m_fileNameLabel->setGeometry( QRect( 0, 70, 100, 30 ) );
      m_fileNameLabel->setAlignment( Qt::AlignCenter );

      m_fileNameEdit = new QLineEdit( m_mainBox );
      m_fileNameEdit->setGeometry( QRect( 100, 72, 170, 25 ) );

      m_starterButton = new QPushButton( tr( "Start hbook writer" ), m_mainBox );
      m_starterButton->setGeometry( QRect( 20, 100, 250, 35 ) );
      m_starterButton->setCheckable( true );
      connect( m_starterButton, SIGNAL( clicked( bool ) ),
               this, SLOT( startApp( bool ) ) );

      m_runner.setExecName( daq::PathResolver::resolve( "cda-hbook-writer",
                                                        "PATH" ) );

   }

   HBookWriterRunner::~HBookWriterRunner() {

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

         QString options;
         options += " -m " + m_msgServerAddress;
         options += " -c " + m_configFileName;
         options += " -v " + QString::number( m_level );
         options += " -e " + m_eventAddress;
         options += " -o " + m_fileNameEdit->text();

         m_logger << msg::DEBUG << "Using options: " << options << msg::endmsg;
         m_runner.setOptions( options );

         if( ! m_runner.start() ) {
            m_logger << msg::ERROR << "Couldn't start HBook writer!" << msg::endmsg;

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Reset" ) );

         } else {
            m_logger << msg::INFO << "HBook writer started" << msg::endmsg;

            m_processStatus->setText( tr( "HBook writer running" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

            m_starterButton->setText( tr( "Stop hbook writer" ) );

            m_fileNameEdit->setReadOnly( true );

            emit running( true );

         }

      } else {

         if( ! m_runner.stop() ) {
            m_logger << msg::ERROR << "The HBook writer could not be stopped "
                     << "successfully" << msg::endmsg;

            m_processStatus->setText( tr( "ERROR" ) );
            QPalette palette( m_processStatus->palette() );
            palette.setColor( QPalette::Active, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            palette.setColor( QPalette::Inactive, QPalette::Foreground,
                              QColor( 150, 10, 10 ) );
            m_processStatus->setPalette( palette );

         } else {
            m_logger << msg::INFO << "HBook writer stopped" << msg::endmsg;

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

} // namespace simple_daq
