// $Id$

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>

// Local include(s):
#include "Statistics.h"

namespace simple_daq {

   Statistics::Statistics( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_server( this ) {

      setMinimumSize( 300, 150 );
      setMaximumSize( 300, 150 );

      m_mainBox = new QGroupBox( tr( "DAQ statistics" ), this );
      m_mainBox->setGeometry( QRect( 5, 5, 290, 140 ) );

      m_readEventsLabel = new QLabel( tr( "Events processed:" ), m_mainBox );
      m_readEventsLabel->setGeometry( QRect( 10, 40, 140, 25 ) );
      m_readEventsLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_readEvents = new QLabel( tr( "n/a" ), m_mainBox );
      m_readEvents->setGeometry( QRect( 160, 40, 100, 25 ) );
      m_readEvents->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      m_eventRateLabel = new QLabel( tr( "Event rate:" ), m_mainBox );
      m_eventRateLabel->setGeometry( QRect( 10, 80, 140, 25 ) );
      m_eventRateLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      m_eventRate = new QLabel( tr( "n/a" ), m_mainBox );
      m_eventRate->setGeometry( QRect( 160, 80, 100, 25 ) );
      m_eventRate->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      connect( &m_server, SIGNAL( statAvailable( const stat::Statistics& ) ),
               this, SLOT( updateStat( const stat::Statistics& ) ) );

      m_rateTimer = new QTimer( this );
      m_rateTimer->setSingleShot( true );

      connect( m_rateTimer, SIGNAL( timeout() ),
               this, SLOT( updateStat() ) );

   }

   Statistics::~Statistics() {

      delete m_readEventsLabel; delete m_readEvents;
      delete m_eventRateLabel; delete m_eventRate;
      delete m_mainBox;

   }

   void Statistics::setStatAddress( const Address& address ) {

      m_server.listen( address );
      return;

   }

   Address Statistics::getStatAddress() const {

      return Address( m_server.serverAddress().toString(), m_server.serverPort() );

   }

   void Statistics::updateStat( const stat::Statistics& stat ) {

      m_readEvents->setText( QString::number( stat.getProcessedEvents() ) );
      m_eventRate->setText( QString::number( static_cast< double >( stat.getProcessedEvents() -
               m_lastStat.getProcessedEvents() ) /
               m_lastStat.getStatTime().msecsTo( stat.getStatTime() ) * 1000 ) ); 

      m_lastStat = stat;

      // If we don't receive an update from cda-camac-reader in 15 seconds, let's
      // declare the rate zero:
      m_rateTimer->start( 15000 );

      return;

   }

   void Statistics::updateStat() {

      m_eventRate->setText( "0" );
      m_lastStat.setStatTime( QTime::currentTime() );

      return;

   }

} // namespace simple_daq
