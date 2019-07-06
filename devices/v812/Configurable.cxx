
// Local include(s).
#include "Configurable.h"

// CDA include(s).
#include "caen/VmeBus.h"
#include "common/errorcheck.h"

// System include(s).
#include <vector>

namespace std {

   /// Helper function for printing std::vector values
   template< typename T >
   std::ostream& operator<<( std::ostream& out, const std::vector< T >& vec ) {

      out << "[";
      for( std::size_t i = 0; i < vec.size(); ++i ) {
         out << vec[ i ];
         if( i + 1 < vec.size() ) {
            out << ", ";
         }
      }
      out << "]";
      return out;
   }

} // namespace std

namespace v812 {

   Configurable::Configurable()
   : Device(), m_logger( "v812::Configurable" ) {

   }

   StatusCode Configurable::configure( const caen::VmeBus& bus ) const {

      // Tell the user what's happening.
      m_logger << msg::INFO << tr( "Configuring device on address: %1" )
                               .arg( m_vmeAddress, 4, 16 ) << msg::endmsg;

      // Construct the 16-bit inhibit word, used to enable/disable channels on
      // the device.
      uint16_t inhibitWord = 0;
      std::vector< size_t > enabledChannels;
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( ! m_channels[ i ] ) {
            continue;
         }
         inhibitWord |= ( 1 << i );
         enabledChannels.push_back( i );
      }
      m_logger << msg::INFO << tr( "Enabled channel(s): " ) << enabledChannels
               << msg::endmsg;

      // Set the inhibit pattern on the device.
      CHECK( bus.write( m_vmeAddress + 0x4a, inhibitWord ) );

      // Set the discriminant thresholds for all enabled channels.
      m_logger << msg::INFO << tr( "Setting the channel thresholds..." )
               << msg::endmsg;
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( ! m_channels[ i ] ) {
            continue;
         }
         CHECK( bus.write( m_vmeAddress + i * 0x2,
                           m_channels[ i ]->getThreshold() ) );
         m_logger << msg::INFO << tr( "  - Channel %1: %2" ).arg( i, 2 )
                  .arg( static_cast< int >( m_channels[ i ]->getThreshold() ) )
                  << msg::endmsg;
      }

      // Set the device level properties.
      CHECK( bus.write( m_vmeAddress + 0x40, m_outputWidth1 ) );
      m_logger << msg::INFO << tr( "Output width (channels 0-7) : %1" )
                               .arg( static_cast< int >( m_outputWidth1 ) )
               << msg::endmsg;
      CHECK( bus.write( m_vmeAddress + 0x42, m_outputWidth2 ) );
      m_logger << msg::INFO << tr( "Output width (channels 8-15): %1" )
                               .arg( static_cast< int >( m_outputWidth2 ) )
               << msg::endmsg;
      CHECK( bus.write( m_vmeAddress + 0x44, m_deadTime1 ) );
      m_logger << msg::INFO << tr( "Dead time (channels 0-7) : %1" )
                               .arg( static_cast< int >( m_deadTime1 ) )
               << msg::endmsg;
      CHECK( bus.write( m_vmeAddress + 0x46, m_deadTime2 ) );
      m_logger << msg::INFO << tr( "Dead time (channels 8-15): %1" )
                               .arg( static_cast< int >( m_deadTime2 ) )
               << msg::endmsg;
      CHECK( bus.write( m_vmeAddress + 0x48, m_majorityThreshold ) );
      m_logger << msg::INFO << tr( "Majority threshold: %1" )
                              .arg( static_cast< int >( m_majorityThreshold ) )
               << msg::endmsg;

      // Return gracefully.
      return StatusCode::SUCCESS;
   }

} // namespace v812
