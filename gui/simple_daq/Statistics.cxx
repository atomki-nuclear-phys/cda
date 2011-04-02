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

      //
      // Set the widget to a fixed size:
      //
      setMinimumSize( 300, 150 );
      setMaximumSize( 300, 150 );

      //
      // Create the box into which everything else is put:
      //
      m_mainBox = new QGroupBox( tr( "DAQ statistics (for current output file)" ), this );
      m_mainBox->setGeometry( QRect( 5, 5, 290, 140 ) );

      //
      // Draw the "Events processed" label:
      //
      m_readEventsLabel = new QLabel( tr( "Events processed:" ), m_mainBox );
      m_readEventsLabel->setGeometry( QRect( 10, 40, 140, 25 ) );
      m_readEventsLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      //
      // Create the label showing the number of processed events:
      //
      m_readEvents = new QLabel( tr( "n/a" ), m_mainBox );
      m_readEvents->setGeometry( QRect( 160, 40, 100, 25 ) );
      m_readEvents->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      //
      // Create the "Event rate" label:
      //
      m_eventRateLabel = new QLabel( tr( "Event rate:" ), m_mainBox );
      m_eventRateLabel->setGeometry( QRect( 10, 80, 140, 25 ) );
      m_eventRateLabel->setAlignment( Qt::AlignRight | Qt::AlignVCenter );

      //
      // Create the label showing the event rate:
      //
      m_eventRate = new QLabel( tr( "n/a Hz" ), m_mainBox );
      m_eventRate->setGeometry( QRect( 160, 80, 100, 25 ) );
      m_eventRate->setAlignment( Qt::AlignLeft | Qt::AlignVCenter );

      //
      // Set up statistics updates:
      //
      connect( &m_server, SIGNAL( statAvailable( const stat::Statistics& ) ),
               this, SLOT( updateStat( const stat::Statistics& ) ) );

      //
      // Set up the timer that resets the statistics when a new stat information
      // isn't received for a given amount of time:
      //
      m_rateTimer = new QTimer( this );
      m_rateTimer->setSingleShot( true );
      connect( m_rateTimer, SIGNAL( timeout() ),
               this, SLOT( updateStat() ) );

   }

   /**
    * The destructor simply deletes all the objects created in the constructor.
    */
   Statistics::~Statistics() {

      delete m_rateTimer;
      delete m_readEventsLabel; delete m_readEvents;
      delete m_eventRateLabel; delete m_eventRate;
      delete m_mainBox;
   }

   /**
    * @param address Address where the widget should listen to DAQ statistics
    */
   void Statistics::setStatAddress( const Address& address ) {

      m_server.listen( address );
      return;
   }

   /**
    * @returns The address on which the widget is listening to DAQ statistics
    */
   Address Statistics::getStatAddress() const {

      return Address( m_server.serverAddress().toString(), m_server.serverPort() );
   }

   /**
    * This function calculates the event processing rate, and displays it alongside
    * the total number of events processed so far.
    *
    * @param stat The latest statistics object
    */
   void Statistics::updateStat( const stat::Statistics& stat ) {

      //
      // Show the number of processed events according to the last
      // stat::Statistics object received, and calculate the event
      // processing rate using the last statistics object:
      //
      m_readEvents->setText( QString::number( stat.getProcessedEvents() ) );
      if( stat.getSource() == m_lastStat.getSource() ) {
         // If the statistics is comign from the same source, then calculate the
         // rate:
         m_eventRate->setText( QString::number( static_cast< double >( stat.getProcessedEvents() -
                   m_lastStat.getProcessedEvents() ) /
                   m_lastStat.getStatTime().msecsTo( stat.getStatTime() ) * 1000.0 ) +
                               " Hz" ); 
      } else {
         // If the statistics is from a new source, wait with calculating the rate:
         m_eventRate->setText( "n/a Hz" );
      }

      // Remember this as the last statistics object:
      m_lastStat = stat;

      // If we don't receive an update from cda-camac-reader in 15 seconds, let's
      // declare the rate zero:
      m_rateTimer->start( 5000 );

      return;
   }

   /**
    * This function resets the event rate to "0 Hz" if no new statistics has been
    * received for a given amount of time.
    */
   void Statistics::updateStat() {

      m_eventRate->setText( "0 Hz" );
      m_lastStat.setStatTime( QTime::currentTime() );

      return;
   }

} // namespace simple_daq
