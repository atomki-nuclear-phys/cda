// $Id$

// STL include(s):
#include <cmath>

// Local include(s):
#include "Processor.h"

namespace dt5740 {

   Processor::Processor()
      : m_fraction( 0.4 ), m_delay( 40 ), m_length( 20 ),
        m_logger( "dt5740::Processor" ) {

   }

   /**
    * This is the main signal reconstruction function of the device's
    * data processing.
    *
    * @param data The raw samples collected from a channel
    * @param result The reconstructed properties of the signal
    * @returns <code>true</code> if the singal reconstruction was successful,
    *          <code>false</code> otherwise
    */
   bool Processor::reconstruct( const std::vector< uint16_t >& data,
                                Result& result ) {

      // Reset the result:
      result.time   = 0.0;
      result.energy = 0.0;

      // Create the transformed data:
      m_trans.resize( data.size(), 0.0 );

      // Loop over all the events:
      for( size_t k = 0; k < data.size(); ++k ) {
         // Create the transformed values for the samples for which it is defined:
         m_trans[ k ] = 0.0;
         if( ( k >= 1 ) && ( k < ( data.size() - m_delay ) ) ) {
            for( int i = 0; i < m_length; ++i ) {
               m_trans[ k ] += ( ( double ) data[ k - 1 ] -
                                 m_fraction * ( ( double ) data[ k - i + m_delay ] ) );
            }
         }
         // Search for the maximum:
         if( data[ k ] > result.energy ) {
            result.energy = data[ k ];
         }
      }

      // Find the zero-crossing of the transformed data. That is assigned as the
      // time for the signal.
      for( size_t i = 0; i < m_trans.size() - 4; ++i ) {
         if( ( m_trans[ i ] < 0.0 ) &&
             ( m_trans[ i + 1 ] < 0.0 ) &&
             ( m_trans[ i + 2 ] > 0.0 ) &&
             ( m_trans[ i + 3 ] > 0.0 ) ) {
            result.time = ( double ) i + 3.0 * ( std::abs( m_trans[ i ] ) /
                                                 std::abs( m_trans[ i + 4 ] ) );
            break;
         }
      }

      return true;
   }

} // namespace dt5740
