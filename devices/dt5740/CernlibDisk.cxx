// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_WS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#else
#   include "common/errorcheck.h"
#   include "cernlib/NTupleMgr.h"
#endif

// Local include(s):
#include "CernlibDisk.h"

namespace dt5740 {

   CernlibDisk::CernlibDisk()
      : m_processor(), m_logger( "dt5740::CernlibDisk" ) {

      // Reset the ntuple map:
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {
            for( int i = 0; i < 2; ++i ) {
               m_ntupleMap[ group ][ channel ][ i ] = -1;
            }
         }
      }
   }

   bool CernlibDisk::initialize( cernlib::NTupleMgr& nmgr ) {

      // Configure the signal processor:
      m_processor.setFraction( m_cfdFraction );
      m_processor.setDelay( m_cfdDelay );
      m_processor.setLength( m_cfdLength );
      m_processor.setSmoothWidth( m_gaussSmoothWidth );

      //
      // Create variables for all the active channels:
      //
      for( int group = 0; group < NUMBER_OF_GROUPS; ++group ) {
         for( int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
              ++channel ) {

            // Check if the channel is active:
            const ChannelConfig* ch = m_groups[ group ].getChannel( channel );
            if( ! ch ) continue;

            // Create the time and energy variables:
            m_ntupleMap[ group ][ channel ][ 0 ] =
               nmgr.addVar( ch->getTimeName() );
            m_ntupleMap[ group ][ channel ][ 1 ] =
               nmgr.addVar( ch->getEnergyName() );
         }
      }

      return true;
   }

   bool CernlibDisk::writeEvent( const ev::Fragment& fragment,
                                 cernlib::NTupleMgr& nmgr ) const {

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
            CHECK( nmgr.setVar( m_ntupleMap[ group ][ channel ][ 0 ],
                                static_cast< float >( res.time ) ) );
            CHECK( nmgr.setVar( m_ntupleMap[ group ][ channel ][ 1 ],
                                static_cast< float >( res.energy ) ) );
         }
      }

      return true;
   }

} // namespace dt5740
