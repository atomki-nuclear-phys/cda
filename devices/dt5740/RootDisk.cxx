// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_WS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/root/NTupleMgr.h"
#else
#   include "common/errorcheck.h"
#   include "root/NTupleMgr.h"
#endif

// Local include(s):
#include "RootDisk.h"

namespace dt5740 {

   RootDisk::RootDisk()
      : m_processor(),
        m_logger( "dt5740::RootDisk" ) {

   }

   bool RootDisk::initialize( root::NTupleMgr& nmgr ) {

      // Configure the signal processor:
      m_processor.setFraction( m_cfdFraction );
      m_processor.setDelay( m_cfdDelay );
      m_processor.setLength( m_cfdLength );

      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Check if the channel is active:
            const ChannelConfig* ch = m_groups[ group ].getChannel( channel );
            if( ! ch ) continue;

            // Declare the time and energy variables:
            CHECK( nmgr.addVar( m_times[ group ][ channel ],
                                ch->getTimeName() ) );
            CHECK( nmgr.addVar( m_energies[ group ][ channel ],
                                ch->getEnergyName() ) );

            // Declare the raw data variable if needed:
            if( m_saveRawNtuple ) {
               m_channelData[ group ][ channel ].resize( getSamples(), 0 );
               CHECK( nmgr.addVar( m_channelData[ group ][ channel ],
                                   ch->getRawName() ) );
            }
         }
      }

      return true;
   }

   bool RootDisk::writeEvent( const ev::Fragment& fragment ) const {

      // Decode the event fragment:
      CHECK( decode( fragment, m_eventInfo, m_eventData ) );

      // The reconstructed object to be used:
      Processor::Result res;

      //
      // Reconstruct the data in all channels, and fill the ntuple variables:
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

            // Fill the time and energy variables for the current event:
            m_times[ group ][ channel ]    = res.time;
            m_energies[ group ][ channel ] = res.energy;

            // Fill the raw data if needed:
            if( m_saveRawNtuple ) {
               m_channelData[ group ][ channel ] =
                  m_eventData.chData[ group * GroupConfig::CHANNELS_IN_GROUP +
                                      channel ];
            }
         }
      }

      return true;
   }

} // namespace dt5740
