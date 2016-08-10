// $Id$

// Qt include(s):
#include <QStackedLayout>
#include <QTabWidget>

// CDA include(s):
#include "common/errorcheck.h"
#include "event/Fragment.h"

// Local include(s):
#include "QtHist.h"

namespace ad2249a {

   QtHist::QtHist( QWidget* parent, Qt::WindowFlags flags )
      : dev::QtHist( parent, flags ),
        m_logger( "dt5740::QtHist" ) {

      // Declare a minimum size for this widget:
      setMinimumSize( 300, 300 );

      // Create the "layout" for the tab widget:
      m_channelLayout.reset( new QStackedLayout( this ) );

      // Set up the tabs for the channels:
      m_channelTab.reset( new QTabWidget() );
      m_channelTab->setMinimumSize( 300, 300 );
      m_channelTab->setMaximumSize( 20000, 20000 );
      m_channelTab->setSizePolicy( QSizePolicy::Expanding,
                                   QSizePolicy::Expanding );
      m_channelTab->setUsesScrollButtons( true );

      // Add the tabs to the layout:
      m_channelLayout->addWidget( m_channelTab.get() );

   }

   StatusCode QtHist::readConfig( QIODevice& dev ) {

      // Read in the configuration using the base class:
      CHECK( Device::readConfig( dev ) );

      // Update the look of the widget:
      CHECK( initialize() );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode QtHist::readConfig( const QDomElement& node ) {

      // Read in the configuration using the base class:
      CHECK( Device::readConfig( node ) );

      // Update the look of the widget:
      CHECK( initialize() );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   bool QtHist::displayEvent( const ev::Fragment& fragment ) {

      // Access the data words:
      const ev::Fragment::Payload_t& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      ev::Fragment::Payload_t::const_iterator dword_itr = dataWords.begin();
      ev::Fragment::Payload_t::const_iterator dword_end = dataWords.end();
      for( ; dword_itr != dword_end; ++dword_itr ) {

         // Decode the data word:
         const int subaddress    = ( *dword_itr >> 24 ) & 0xff;
         const unsigned int data = ( *dword_itr & 0xffffff );

         // Check that the decoded information makes sense:
         if( ! ( ( subaddress >= 0 ) &&
                 ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
                 m_histograms[ subaddress ] ) ) {
            REPORT_ERROR( tr( "Received data word from unknown channel" ) );
            return false;
         }

         // Fill the histogram:
         m_histograms[ subaddress ]->fill( ( double ) data, 1.0 );
      }

      return true;
   }

   bool QtHist::initialize() {

      // Reset the widget first:
      CHECK( reset() );

      // Create a new tab for each channel:
      for( int channel = 0; channel < NUMBER_OF_SUBADDRESSES;
           ++channel ) {

         // Check if the channel is active:
         const ChannelConfig* ch = m_channels[ channel ].get();
         if( ! ch ) continue;

         // Create a histogram for this channel:
         m_histograms[ channel ].reset(
            new moni::Histogram( ch->getName(), ch->getNumberOfChannels(),
                                 ch->getLowerBound(), ch->getUpperBound() ) );

         // Add it as a new tab:
         m_channelTab->addTab( m_histograms[ channel ].get(),
                               tr( "channel %1" ).arg( channel ) );
      }

      return true;
   }

   bool QtHist::reset() {

      // Clear the tabs. This does not delete them.
      m_channelTab->clear();

      // Delete all conditionally created objects:
      for( int channel = 0; channel < NUMBER_OF_SUBADDRESSES;
           ++channel ) {
         m_histograms[ channel ].reset();
      }

      return true;
   }

} // namespace ad2249a
