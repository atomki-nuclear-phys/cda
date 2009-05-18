// $Id$

// Qt include(s):
#include <QtCore/QTimer>
#include <QtGui/QLabel>

// Local include(s):
#include "RateWindow.h"
#include "RateWidget.h"

namespace stat {

   RateWindow::RateWindow( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_currentRate( 0.0 ) {

      resize( 450, 200 );
      setMinimumSize( 450, 200 );
      setMaximumSize( 450, 200 );

      m_rateWidget = new RateWidget( this );
      m_rateWidget->setGeometry( QRect( 10, 10, 150, 180 ) );

      m_sourceNameLabel = new QLabel( tr( "Source:" ), this );
      m_sourceNameLabel->setGeometry( QRect( 160, 40, 100, 25 ) );
      m_sourceNameLabel->setAlignment( Qt::AlignRight );

      m_sourceName = new QLabel( tr( "n/a" ), this );
      m_sourceName->setGeometry( QRect( 270, 40, 160, 25 ) );
      m_sourceName->setAlignment( Qt::AlignLeft );

      m_processedEventsLabel = new QLabel( tr( "Processed events:" ), this );
      m_processedEventsLabel->setGeometry( QRect( 160, 70, 150, 25 ) );
      m_processedEventsLabel->setAlignment( Qt::AlignRight );

      m_processedEvents = new QLabel( tr( "n/a" ), this );
      m_processedEvents->setGeometry( QRect( 320, 70, 130, 25 ) );
      m_processedEvents->setAlignment( Qt::AlignLeft );

      m_eventRateLabel = new QLabel( tr( "Event rate:" ), this );
      m_eventRateLabel->setGeometry( QRect( 160, 100, 150, 25 ) );
      m_eventRateLabel->setAlignment( Qt::AlignRight );

      m_eventRate = new QLabel( tr( "n/a Hz" ), this );
      m_eventRate->setGeometry( QRect( 320, 100, 130, 25 ) );
      m_eventRate->setAlignment( Qt::AlignLeft );

      m_rateTimer = new QTimer( this );
      m_rateTimer->setInterval( 1500 );
      m_rateTimer->setSingleShot( false );
      connect( m_rateTimer, SIGNAL( timeout() ),
               this, SLOT( updateRate() ) );
      m_rateTimer->start();

      m_resetTimer = new QTimer( this );
      m_resetTimer->setInterval( 15000 );
      m_resetTimer->setSingleShot( true );
      connect( m_resetTimer, SIGNAL( timeout() ),
               this, SLOT( resetStatistics() ) );

   }

   RateWindow::~RateWindow() {

      delete m_rateWidget;
      delete m_sourceNameLabel; delete m_sourceName;
      delete m_processedEventsLabel; delete m_processedEvents;
      delete m_eventRateLabel; delete m_eventRate;

   }

   void RateWindow::updateStatistics( const Statistics& stat ) {

      if( stat.getSource() == m_lastStat.getSource() ) {
         m_currentRate = static_cast< double >( stat.getProcessedEvents() -
                                                m_lastStat.getProcessedEvents() ) /
            m_lastStat.getStatTime().msecsTo( stat.getStatTime() ) * 1000.0;
         m_eventRate->setText( QString::number( m_currentRate ) + " Hz" );
      } else {
         m_currentRate = 0.0;
         m_eventRate->setText( "n/a Hz" );
      }

      m_sourceName->setText( stat.getSource() );
      m_lastStat = stat;

      m_resetTimer->start();

      return;

   }

   void RateWindow::resetStatistics() {

      m_currentRate = 0.0;
      m_eventRate->setText( "0 Hz" );
      m_lastStat.setStatTime( QTime::currentTime() );
      return;

   }

   void RateWindow::updateRate() {

      emit newRateAvailable( m_currentRate );
      m_rateWidget->setNewRate( m_currentRate );

      return;

   }


} // namespace stat
