// $Id$

// STL include(s):
#include <cmath>

// Local include(s):
#include "Processor.h"

namespace dt5740 {

   Processor::Processor()
      : m_fraction( 0.2 ), m_delay( 20 ), m_length( 10 ), m_smoothWidth( 0.0 ),
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

      // Create the smoothed/transformed data:
      m_smooth.resize( data.size(), 0.0 );
      m_trans.resize( data.size(), 0.0 );

      // In the first loop smooth the data if needed:
      if( std::abs( m_smoothWidth ) > 0.001 ) {
         for( size_t i = 0; i < data.size(); ++i ) {
            // Start from zero:
            m_smooth[ i ] = 0.0;
            for( size_t j = 0; j < data.size(); ++j ) {
               // The distance of this point from the one being smoothed:
               const int dist = ( static_cast< int >( j ) -
                                  static_cast< int >( i ) );
               // Add the contribution of this point to the one being smoothed:
               m_smooth[ i ] += ( gauss( dist ) * 
                                  static_cast< double >( data[ j ] ) );
            }
         }
      } else {
         for( size_t i = 0; i < data.size(); ++i ) {
            m_smooth[ i ] = static_cast< double >( data[ i ] );
         }
      }

      // Now do the CFD transformation:
      for( size_t k = 0; k < m_smooth.size(); ++k ) {
         // Create the transformed values for the samples for which it is defined:
         m_trans[ k ] = 0.0;
         if( ( k >= 1 ) && ( k < ( m_smooth.size() - m_delay ) ) ) {
            for( int i = 0; i < m_length; ++i ) {
               m_trans[ k ] += ( m_smooth[ k - 1 ] -
                                 m_fraction * m_smooth[ k - i + m_delay ] );
            }
         }
         // Search for the maximum in the same loop:
         if( m_smooth[ k ] > result.energy ) {
            result.energy = m_smooth[ k ];
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

   double Processor::getFraction() const {

      return m_fraction;
   }

   void Processor::setFraction( double value ) {

      m_fraction = value;
      return;
   }

   int Processor::getDelay() const {

      return m_delay;
   }

   void Processor::setDelay( int value ) {

      m_delay = value;
      return;
   }

   int Processor::getLength() const {

      return m_length;
   }

   void Processor::setLength( int value ) {

      m_length = value;
      return;
   }

   double Processor::getSmoothWidth() const {

      return m_smoothWidth;
   }

   void Processor::setSmoothWidth( double value ) {

      m_smoothWidth = value;
      return;
   }

   /**
    * A Gaussian distribution is used for smoothing the digitised data points.
    * The used distribution is always centered at zero, and its width (sigma)
    * is defined by <code>m_smoothWidth</code>. If the area is set to zero, it
    * means that no signal smoothing needs to happen.
    *
    * @param x Distance of measurement point from the point to be smoothed
    */
   double Processor::gauss( int x ) const {

      // First let's deal with the case when no smoothing needs to happen:
      if( std::abs( m_smoothWidth ) < 0.001 ) {
         if( x == 0 ) {
            return 1.0;
         } else {
            return 0.0;
         }
      }

      // For distances larger than 5 sigma, let's not do the calculation:
      if( std::abs( static_cast< double >( x ) ) >
          5 * std::abs( m_smoothWidth ) ) {
         return 0.0;
      }

      // I've stolen this calculation from TMath::Gaus in the ROOT source code:
      const double arg = static_cast< double >( x ) / m_smoothWidth;
      const double res = std::exp( -0.5 * arg * arg );
      static const double SQRT_2PI = 2.50662827463100024;
      return res / ( SQRT_2PI * m_smoothWidth );
   }

} // namespace dt5740
