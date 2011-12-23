// $Id$

// Qt include(s):
#include <QtGui/QStackedLayout>
#include <QtGui/QScrollArea>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdadaq/moni/Histogram.h"
#else
#   include "common/errorcheck.h"
#   include "moni/Histogram.h"
#endif

// Local include(s):
#include "QtHist.h"

namespace dt5740 {

   QtHist::QtHist( QWidget* parent, Qt::WindowFlags flags )
      : dev::QtHist( parent, flags ), m_lastUpdate( QTime::currentTime() ),
        m_processor(), m_logger( "dt5740::QtHist" ) {

      // Reset all the object pointers as a start:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            m_layouts[ group ][ channel ] = 0;
            m_areas[ group ][ channel ] = 0;
            m_widgets[ group ][ channel ] = 0;
            /*
            for( int i = 0; i < 2; ++i ) {
               m_widgets[ group ][ channel ][ i ] = 0;
            }
            */
            for( int i = 0; i < 3; ++i ) {
               m_histograms[ group ][ channel ][ i ] = 0;
            }
         }
      }

      // Create the "layout" for the tab widget:
      m_channelLayout = new QStackedLayout( this );

      // Set up the tabs for the channels:
      m_channelTab = new QTabWidget();
      m_channelTab->setMinimumSize( 300, 300 );
      m_channelTab->setMaximumSize( 20000, 20000 );
      m_channelTab->setSizePolicy( QSizePolicy::Expanding,
                                   QSizePolicy::Expanding );

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

      // Decode the event fragment:
      CHECK( decode( fragment, m_eventInfo, m_eventData ) );

      // The reconstructed object to be used:
      Processor::Result res;

      //
      // Reconstruct the data in all channels, and fill the histograms:
      //
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Check if the channel is active:
            const ChannelConfig* ch = m_groups[ group ].getChannel( channel );
            if( ! ch ) continue;

            // Update the contents of the raw histogram every 5 seconds:
            if( m_lastUpdate.secsTo( QTime::currentTime() ) > 5 ) {
               m_histograms[ group ][ channel ][ 0 ]->reset();
               for( int i = 0; i < getSamples(); ++i ) {
                  m_histograms[ group ][ channel ][ 0 ]->fill( i,
                                                               m_eventData.chData[ group *
                                                                                   GroupConfig::CHANNELS_IN_GROUP +
                                                                                   channel ][ i ] );
               }
               m_lastUpdate = QTime::currentTime();
            }

            // Reconstruct the data of this channel:
            CHECK( m_processor.reconstruct( m_eventData.chData[ group *
                                                                GroupConfig::CHANNELS_IN_GROUP +
                                                                channel ], res ) );

            // Fill the decoded information:
            m_histograms[ group ][ channel ][ 1 ]->fill( res.time );
            m_histograms[ group ][ channel ][ 2 ]->fill( res.energy );
         }
      }

      return true;
   }

   bool QtHist::initialize() {

      // Reset the widget first:
      CHECK( reset() );

      // Create a new tag for each channel:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Check if the channel is active:
            const ChannelConfig* ch = m_groups[ group ].getChannel( channel );
            if( ! ch ) continue;

            // Convenience pointers:
            QWidget*     widget = m_widgets[ group ][ channel ];
            QScrollArea* area   = m_areas[ group ][ channel ];
            QVBoxLayout* layout = m_layouts[ group ][ channel ];
            moni::Histogram** hists = m_histograms[ group ][ channel ];

            // Create a widget for the tab:
            widget = new QWidget();

            // Create the scrolled widget:
            /*
            widgets[ 1 ] = new QWidget();
            widgets[ 1 ]->setMinimumSize( 250, 600 );
            */

            // Create the scroll area:
            /*
            area = new QScrollArea( widgets[ 0 ] );
            area->setWidget( widgets[ 1 ] );
            */

            // Create the simple layout for the tab:
            layout = new QVBoxLayout( widget );

            // Create the 3 histograms:
            hists[ 0 ] = new moni::Histogram( ch->getRawName(), getSamples(),
                                              -0.5, getSamples() - 0.5 );
            layout->addWidget( hists[ 0 ] );

            hists[ 1 ] = new moni::Histogram( ch->getTimeName(), ch->getTimeNumberOfChannels(),
                                              ch->getTimeLowerBound(),
                                              ch->getTimeUpperBound() );
            layout->addWidget( hists[ 1 ] );

            hists[ 2 ] = new moni::Histogram( ch->getEnergyName(), ch->getEnergyNumberOfChannels(),
                                              ch->getEnergyLowerBound(),
                                              ch->getEnergyUpperBound() );
            layout->addWidget( hists[ 2 ] );

            // Add the top widget as a tab:
            m_channelTab->addTab( widget,
                                  tr( "Group %1, channel %2 (%3)" )
                                  .arg( group ).arg( channel )
                                  .arg( ch->getRawName() ) );

         }
      }

      return true;
   }

   bool QtHist::reset() {

      // Clear the tabs. This does not delete them.
      m_channelTab->clear();

      // Delete all conditionally created objects:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            for( int i = 0; i < 3; ++i ) {
               if( m_histograms[ group ][ channel ][ i ] ) {
                  delete m_histograms[ group ][ channel ][ i ];
                  m_histograms[ group ][ channel ][ i ] = 0;
               }
            }
            if( m_layouts[ group ][ channel ] ) {
               delete m_layouts[ group ][ channel ];
               m_layouts[ group ][ channel ] = 0;
            }
            if( m_widgets[ group ][ channel ] ) {
               delete m_widgets[ group ][ channel ];
               m_widgets[ group ][ channel ] = 0;
            }
            if( m_areas[ group ][ channel ] ) {
               delete m_areas[ group ][ channel ];
               m_areas[ group ][ channel ] = 0;
            }
            /*
            if( m_widgets[ group ][ channel ][ 0 ] ) {
               delete m_widgets[ group ][ channel ][ 0 ];
               m_widgets[ group ][ channel ][ 0 ] = 0;
            }
            */
         }
      }

      return true;
   }

} // namespace dt5740
