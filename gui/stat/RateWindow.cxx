// $Id$

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QLabel>
#include <QCloseEvent>

// Local include(s):
#include "RateWindow.h"
#include "RateWidget.h"

namespace cdastat {

   //
   // Specify the value(s) of the constant(s):
   //
   const quint32 RateWindow::RATE_UPDATE_TIMEOUT = 1500;
   const quint32 RateWindow::RATE_RESET_TIMEOUT  = 5000;

   /**
    * The constructor creates the layout of the window, and sets up the
    * updating of the rate values.
    */
   RateWindow::RateWindow( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_currentRate( 0.0 ) {

      // Set the window to a fixed size:
      resize( 450, 200 );
      setMinimumSize( 450, 200 );
      setMaximumSize( 450, 200 );

      //
      // Create the custom widget displaying the previous event processing rates:
      //
      m_rateWidget = new RateWidget( this );
      m_rateWidget->setGeometry( QRect( 10, 10, 150, 180 ) );

      //
      // Create the labels displaying various values of the statistics information:
      //
      m_sourceNameLabel = new QLabel( tr( "Source:" ), this );
      m_sourceNameLabel->setGeometry( QRect( 160, 40, 60, 25 ) );
      m_sourceNameLabel->setAlignment( Qt::AlignRight );

      m_sourceName = new QLabel( tr( "n/a" ), this );
      m_sourceName->setGeometry( QRect( 230, 40, 220, 25 ) );
      m_sourceName->setAlignment( Qt::AlignLeft );

      m_processedEventsLabel = new QLabel( tr( "Processed events:" ), this );
      m_processedEventsLabel->setGeometry( QRect( 160, 70, 130, 25 ) );
      m_processedEventsLabel->setAlignment( Qt::AlignRight );

      m_processedEvents = new QLabel( tr( "n/a" ), this );
      m_processedEvents->setGeometry( QRect( 300, 70, 150, 25 ) );
      m_processedEvents->setAlignment( Qt::AlignLeft );

      m_eventRateLabel = new QLabel( tr( "Event rate:" ), this );
      m_eventRateLabel->setGeometry( QRect( 160, 100, 130, 25 ) );
      m_eventRateLabel->setAlignment( Qt::AlignRight );

      m_eventRate = new QLabel( tr( "n/a Hz" ), this );
      m_eventRate->setGeometry( QRect( 300, 100, 150, 25 ) );
      m_eventRate->setAlignment( Qt::AlignLeft );

      //
      // Create the timers used in the window updating:
      //
      m_rateTimer = new QTimer( this );
      m_rateTimer->setInterval( RATE_UPDATE_TIMEOUT );
      m_rateTimer->setSingleShot( false );
      connect( m_rateTimer, SIGNAL( timeout() ),
               this, SLOT( updateRate() ) );
      m_rateTimer->start();

      m_resetTimer = new QTimer( this );
      m_resetTimer->setInterval( RATE_RESET_TIMEOUT );
      m_resetTimer->setSingleShot( true );
      connect( m_resetTimer, SIGNAL( timeout() ),
               this, SLOT( resetStatistics() ) );
   }

   /**
    * The destructor deletes all the objects created in the constructor.
    */
   RateWindow::~RateWindow() {

      delete m_rateWidget;
      delete m_sourceNameLabel; delete m_sourceName;
      delete m_processedEventsLabel; delete m_processedEvents;
      delete m_eventRateLabel; delete m_eventRate;
      delete m_rateTimer; delete m_resetTimer;
   }

   /**
    * @returns The source of the last statistics object received
    */
   const QString& RateWindow::getSource() const {

      return m_lastStat.getSource();
   }

   /**
    * Since stat::MDIView has to clean up after a sub-window is closed, this
    * class signals just before it is closed, what is going to happen.
    * This is the re-implementation of a QWidget function, which first
    * signals that this window is about to be closed, then accepts the
    * close request.
    */
   void RateWindow::closeEvent( QCloseEvent* event ) {

      emit aboutToClose( m_lastStat.getSource() );
      event->accept();

      return;
   }

   /**
    * This slot can be used by the code using this window class to update the
    * displayed statistics information with the one given to it.
    *
    * @param stat The statistics object to update the displayed information with
    */
   void RateWindow::updateStatistics( const Statistics& stat ) {

      //
      // Calculate the event processing rate:
      //
      if( stat.getSource() == m_lastStat.getSource() ) {
         m_currentRate = ( stat == m_lastStat ? 0.0 :
                           static_cast< qreal >( stat.getProcessedEvents() -
                                                 m_lastStat.getProcessedEvents() ) /
                           m_lastStat.getStatTime().msecsTo( stat.getStatTime() ) * 1000.0 );
         m_eventRate->setText( QString::number( m_currentRate ) + " Hz" );
      } else {
         m_currentRate = 0.0;
         m_eventRate->setText( tr( "n/a Hz" ) );
      }

      //
      // Display the other statistics information:
      //
      m_sourceName->setText( stat.getSource() );
      m_processedEvents->setText( QString::number( stat.getProcessedEvents() ) );

      // Remember this object as the last statistics object received:
      m_lastStat = stat;

      // Start the rate reset timeout:
      m_resetTimer->start();

      return;
   }

   /**
    * This slot resets the event processing rate after no statistics information has
    * been received for RATE_RESET_TIMEOUT length of time.
    */
   void RateWindow::resetStatistics() {

      m_currentRate = 0.0;
      m_eventRate->setText( "0 Hz" );
      m_lastStat.setStatTime( QTime::currentTime() );

      return;
   }

   /**
    * This slot is used internally to update the RateWidget with a new rate value,
    * and to emit a newRateAvailable signal to all the receivers.
    */
   void RateWindow::updateRate() {

      emit newRateAvailable( m_lastStat.getSource(), m_currentRate );
      m_rateWidget->setNewRate( m_currentRate );

      return;
   }


} // namespace cdastat
