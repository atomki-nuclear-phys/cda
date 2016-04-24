// $Id$

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "QtHist.h"

namespace v775 {

   QtHist::QtHist( QWidget* parent, Qt::WindowFlags flags )
      : dev::QtHist( parent, flags ),
        m_logger( "v775::QtHist" ) {

      // Declare a minimum size for this widget:
      setMinimumSize( 250, 250 );

      // Create the "layout" for the tab widget:
      m_channelLayout.reset( new QStackedLayout( this ) );

      // Set up the tabs for the channels:
      m_channelTab.reset( new QTabWidget() );
      m_channelTab->setMinimumSize( 250, 250 );
      m_channelTab->setSizePolicy( QSizePolicy::Expanding,
                                   QSizePolicy::Expanding );
      m_channelTab->setUsesScrollButtons( true );

      // Add the tabs to the layout:
      m_channelLayout->addWidget( m_channelTab.get() );
   }

   QtHist::~QtHist() {

      // To avoid ownership issues between the tab widget and our unique
      // pointers:
      for( auto& hist : m_histograms ) {
         hist.reset();
      }
   }

   bool QtHist::readConfig( QIODevice& dev ) {

      // Read in the configuration using the base class:
      CHECK( Device::readConfig( dev ) );

      // Update the look of the widget:
      CHECK( initialize() );

      // Return gracefully:
      return true;
   }

   bool QtHist::readConfig( const QDomElement& node ) {

      // Read in the configuration using the base class:
      CHECK( Device::readConfig( node ) );

      // Update the look of the widget:
      CHECK( initialize() );

      // Return gracefully:
      return true;
   }

   bool QtHist::displayEvent( const ev::Fragment& fragment ) {

      // Process the received fragment:
      for( auto data : fragment.getDataWords() ) {

         // Decode the word:
         ChannelData cd( data );

         // Make sure that this is a valid channel:
         const size_t channel = cd.channel();
         if( channel >= NUMBER_OF_CHANNELS ) {
            REPORT_ERROR( tr( "Unexpected channel number found: %1" )
                          .arg( channel ) );
            return false;
         }

         // Check if we need to display it:
         if( ! m_histograms[ channel ] ) {
            continue;
         }

         // Fill the histogram with the data:
         m_histograms[ channel ]->fill( cd.data() );
      }

      // Return gracefully:
      return true;
   }

   QString QtHist::windowTitle() const {

      return ( tr( "V775 on address %1" )
               .arg( m_vmeAddress, 4, 16, QChar( '0' ) ) );
   }

   bool QtHist::initialize() {

      // First of all, clear all the current tabs. If they exist.
      m_channelTab->clear();
      for( auto& hist : m_histograms ) {
         hist.reset();
      }

      // Create all the needed histograms:
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {

         // Skip inexistent channels:
         if( ! m_channels[ i ] ) {
            continue;
         }

         // Create the histogram:
         m_histograms[ i ].reset(
            new moni::Histogram( m_channels[ i ]->getName(),
                                 m_channels[ i ]->getNBins(),
                                 m_channels[ i ]->getLowerBound(),
                                 m_channels[ i ]->getUpperBound() ) );

         // Add it as a tab:
         m_channelTab->addTab( m_histograms[ i ].get(),
                               tr( "Channel %1 (%2)" ).arg( i + 1 )
                               .arg( m_channels[ i ]->getName() ) );
      }

      // Return gracefully:
      return true;
   }

} // namespace v775
