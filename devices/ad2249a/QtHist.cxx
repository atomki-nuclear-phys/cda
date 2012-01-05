// $Id$

// Qt include(s):
#include <QtGui/QStackedLayout>
#include <QtGui/QTabWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/event/Fragment.h"
#   include "cdadaq/moni/Histogram.h"
#else
#   include "common/errorcheck.h"
#   include "event/Fragment.h"
#   include "moni/Histogram.h"
#endif

// Local include(s):
#include "QtHist.h"

namespace ad2249a {

   QtHist::QtHist( QWidget* parent, Qt::WindowFlags flags )
      : dev::QtHist( parent, flags ),
        m_logger( "dt5740::QtHist" ) {

      // Declare a minimum size for this widget:
      setMinimumSize( 300, 300 );

      // Reset all the object pointers as a start:
      for( int channel = 0; channel < NUMBER_OF_SUBADDRESSES;
           ++channel ) {
         m_histograms[ channel ] = 0;
      }

      // Create the "layout" for the tab widget:
      m_channelLayout = new QStackedLayout( this );

      // Set up the tabs for the channels:
      m_channelTab = new QTabWidget();
      m_channelTab->setMinimumSize( 300, 300 );
      m_channelTab->setMaximumSize( 20000, 20000 );
      m_channelTab->setSizePolicy( QSizePolicy::Expanding,
                                   QSizePolicy::Expanding );
      m_channelTab->setUsesScrollButtons( true );

      // Add the tabs to the layout:
      m_channelLayout->addWidget( m_channelTab );

   }

   QtHist::~QtHist() {

      reset();

      delete m_channelTab;
      delete m_channelLayout;
   }

   bool QtHist::readConfig( QIODevice* dev ) {

      // Read in the configuration using the base class:
      CHECK( Device::readConfig( dev ) );

      // Update the look of the widget:
      CHECK( initialize() );

      return true;
   }

   bool QtHist::readConfig( const QDomElement& node ) {

      // Read in the configuration using the base class:
      CHECK( Device::readConfig( node ) );

      // Update the look of the widget:
      CHECK( initialize() );

      return true;
   }

   bool QtHist::displayEvent( const ev::Fragment& fragment ) {

      // Access the data words:
      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      for( std::vector< uint32_t >::const_iterator dword = dataWords.begin();
           dword != dataWords.end(); ++dword ) {

         // Decode the data word:
         const int subaddress    = ( *dword >> 24 ) & 0xff;
         const unsigned int data = ( *dword & 0xffffff );

         // Check that the decoded information makes sense:
         if( ! ( ( subaddress >= 0 ) && ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
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
         const ChannelConfig* ch = m_channels[ channel ];
         if( ! ch ) continue;

         // Create a histogram for this channel:
         m_histograms[ channel ] =
            new moni::Histogram( ch->getName(), ch->getNumberOfChannels(),
                                 ch->getLowerBound(), ch->getUpperBound() );

         // Add it as a new tab:
         m_channelTab->addTab( m_histograms[ channel ],
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
         if( m_histograms[ channel ] ) {
            delete m_histograms[ channel ];
            m_histograms[ channel ] = 0;
         }
      }

      return true;
   }

} // namespace ad2249a
