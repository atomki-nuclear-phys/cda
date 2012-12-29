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
#include "RawHistogram.h"

namespace dt5740 {

   QtHist::QtHist( QWidget* parent, Qt::WindowFlags flags )
      : dev::QtHist( parent, flags ),
        m_logger( "dt5740::QtHist" ) {

      // Declare a minimum size for this widget:
      setMinimumSize( 220, 450 );

      // Reset all the object pointers as a start:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            m_layouts[ group ][ channel ] = 0;
            m_widgets[ group ][ channel ] = 0;
            m_rawHistograms[ group ][ channel ] = 0;
            for( int i = 0; i < 2; ++i ) {
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

            // Reconstruct the data of this channel:
            CHECK( m_rawHistograms[ group ][ channel ]->reconstruct(
                                     m_eventData.chData[ group *
                                                         GroupConfig::CHANNELS_IN_GROUP +
                                                         channel ], res ) );

            // Fill the decoded information:
            m_histograms[ group ][ channel ][ 0 ]->fill( res.time );
            m_histograms[ group ][ channel ][ 1 ]->fill( res.energy );
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

            // Create a widget for the tab:
            m_widgets[ group ][ channel ] = new QWidget();

            // Create the simple layout for the tab:
            m_layouts[ group ][ channel ] =
               new QVBoxLayout( m_widgets[ group ][ channel ] );

            // Create the 3 histograms:
            m_rawHistograms[ group ][ channel ] =
               new RawHistogram( ch->getRawName(), getSamples(),
                                 -0.5, getSamples() - 0.5, 5000 );
            m_rawHistograms[ group ][ channel ]->setFraction( m_cfdFraction );
            m_rawHistograms[ group ][ channel ]->setDelay( m_cfdLength );
            m_rawHistograms[ group ][ channel ]->setLength( m_cfdLength );
            m_rawHistograms[ group ][ channel ]->setSmoothWidth( m_gaussSmoothWidth );
            m_layouts[ group ][ channel ]->addWidget( m_rawHistograms[ group ][ channel ] );

            m_histograms[ group ][ channel ][ 0 ] =
               new moni::Histogram( ch->getTimeName(), ch->getTimeNumberOfChannels(),
                                    ch->getTimeLowerBound(),
                                    ch->getTimeUpperBound() );
            m_histograms[ group ][ channel ][ 0 ]->setHistColor( QColor( Qt::darkBlue ) );
            m_layouts[ group ][ channel ]->addWidget( m_histograms[ group ][ channel ][ 0 ] );

            m_histograms[ group ][ channel ][ 1 ] =
               new moni::Histogram( ch->getEnergyName(), ch->getEnergyNumberOfChannels(),
                                    ch->getEnergyLowerBound(),
                                    ch->getEnergyUpperBound() );
            m_histograms[ group ][ channel ][ 1 ]->setHistColor( QColor( Qt::darkGreen ) );
            m_layouts[ group ][ channel ]->addWidget( m_histograms[ group ][ channel ][ 1 ] );

            // Add the top widget as a tab:
            m_channelTab->addTab( m_widgets[ group ][ channel ],
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
            if( m_rawHistograms[ group ][ channel ] ) {
               delete m_rawHistograms[ group ][ channel ];
               m_rawHistograms[ group ][ channel ] = 0;
            }
            for( int i = 0; i < 2; ++i ) {
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
         }
      }

      return true;
   }

} // namespace dt5740
