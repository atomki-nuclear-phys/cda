// $Id$

// CDA include(s):
#include "cernlib/HistMgr.h"

// Local include(s):
#include "CernlibHist.h"

namespace v785 {

   CernlibHist::CernlibHist()
      : Device(), m_histMap(), m_logger( "v785::CernlibHist" ) {

   }

   bool CernlibHist::initialize( cernlib::HistMgr& hmgr ) {

      // Book histograms for all the channels:
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {

         // Skip the inactive channels:
         if( ! m_channels[ i ] ) {
            continue;
         }

         // Book the histogram:
         m_histMap[ i ] = hmgr.book_1d( tr( "%1 (V785 on address %2, "
                                            "channel %3)" )
                                        .arg( m_channels[ i ]->getName() )
                                        .arg( m_vmeAddress, 4, 16,
                                              QChar( '0' ) ).arg( i )
                                        .toLatin1().constData(),
                                        m_channels[ i ]->getNBins(),
                                        m_channels[ i ]->getLowerBound(),
                                        m_channels[ i ]->getUpperBound() );
         REPORT_VERBOSE( tr( "Booked histogram for channel %1 with ID %2" )
                         .arg( i ).arg( m_histMap[ i ] ) );
      }

      // Return gracefully:
      return true;
   }

   bool CernlibHist::displayEvent( const ev::Fragment& fragment,
                                   const cernlib::HistMgr& hmgr ) const {

      // Process the received fragment:
      for( auto data : fragment.getDataWords() ) {

         // Decode the word:
         ChannelData cd( data );

         // Make sure that this is a valid channel:
         const size_t channel = cd.channel();
         if( ( channel >= NUMBER_OF_CHANNELS ) ||
             ( ! m_channels[ channel ] ) ) {
            REPORT_ERROR( tr( "Unexpected channel number found: %1" )
                          .arg( channel ) );
            return false;
         }

         // Fill the histogram:
         hmgr.fill_1d( m_histMap[ channel ], cd.data() );
      }

      // Return gracefully:
      return true;
   }

} // namespace v785
