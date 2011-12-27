// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_WS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/cernlib/HistMgr.h"
#else
#   include "common/errorcheck.h"
#   include "cernlib/HistMgr.h"
#endif

// Local include(s):
#include "CernlibHist.h"

namespace dt5740 {

   CernlibHist::CernlibHist()
      : dev::CernlibHist(), Device(),
        m_processor(),
        m_logger( "dt5740::CernlibHist" ) {

      // Reset the histogram map:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            for( int i = 0; i < 2; ++i ) {
               m_histMap[ group ][ channel ][ i ] = -1;
            }
         }
      }
   }

   bool CernlibHist::initialize( cernlib::HistMgr& hmgr ) {

      // Configure the signal processor:
      m_processor.setFraction( m_cfdFraction );
      m_processor.setDelay( m_cfdDelay );
      m_processor.setLength( m_cfdLength );

      //
      // Book histograms for all the active channels:
      //
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Check if the channel is active:
            const ChannelConfig* ch = m_groups[ group ].getChannel( channel );
            if( ! ch ) continue;

            // Create a fancy name for the time histogram:
            char name[ 80 ];
            sprintf( name, "%s (%s with device ID %i, channel %i)",
                     ch->getTimeName().toLatin1().constData(),
                     deviceName().toLatin1().constData(), getID(),
                     group * GroupConfig::CHANNELS_IN_GROUP + channel );

            // Book the time histogram:
            m_histMap[ group ][ channel ][ 0 ] =
               hmgr.book_1d( name,
                             ch->getTimeNumberOfChannels(),
                             ch->getTimeLowerBound(),
                             ch->getTimeUpperBound() );

            // Create a fancy name for the energy histogram:
            sprintf( name, "%s (%s with device ID %i, channel %i)",
                     ch->getEnergyName().toLatin1().constData(),
                     deviceName().toLatin1().constData(), getID(),
                     group * GroupConfig::CHANNELS_IN_GROUP + channel );

            // Book the energy histogram:
            m_histMap[ group ][ channel ][ 1 ] =
               hmgr.book_1d( name,
                             ch->getEnergyNumberOfChannels(),
                             ch->getEnergyLowerBound(),
                             ch->getEnergyUpperBound() );

            REPORT_VERBOSE( tr( "Created time and energy monitoring histograms for "
                                "channel with raw name %1" ).arg( ch->getRawName() ) );
         }
      }

      return true;
   }

   bool CernlibHist::displayEvent( const ev::Fragment& fragment,
                                   const cernlib::HistMgr& hmgr ) const {

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
            CHECK( m_processor.reconstruct( m_eventData.chData[ group *
                                                                GroupConfig::CHANNELS_IN_GROUP +
                                                                channel ], res ) );

            // Fill the histograms with the reconstructed data:
            hmgr.fill_1d( m_histMap[ group ][ channel ][ 0 ], res.time,   1.0 );
            hmgr.fill_1d( m_histMap[ group ][ channel ][ 1 ], res.energy, 1.0 );
         }
      }

      return true;
   }

} // namespace dt5740
