// $Id$

// System include(s):
#include <cmath>

// Qt include(s):
#include <QtCore/QLine>
#include <QtGui/QPainter>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#else
#   include "common/errorcheck.h"
#endif

// Local include(s):
#include "RawHistogram.h"

namespace dt5740 {

   RawHistogram::RawHistogram( QWidget* parent, Qt::WindowFlags flags )
      : moni::Histogram( parent, flags ),
        Processor(),
        m_time( 0.0 ), m_energy( 0.0 ),
        m_transScale( 1.0 ) {

   }

   RawHistogram::RawHistogram( const QString& title, int bins, double low,
                               double up, int refreshTimeout,
                               QWidget* parent, Qt::WindowFlags flags )
      : moni::Histogram( title, bins, low, up, refreshTimeout,
                         parent, flags ),
        Processor(),
        m_time( 0.0 ), m_energy( 0.0 ),
        m_transScale( 1.0 ) {

   }

   bool RawHistogram::reconstruct( const std::vector< uint16_t >& data,
                                   Processor::Result& res ) {

      // Reconstruct the signal using the base class:
      CHECK( Processor::reconstruct( data, res ) );

      // Store the interesting values:
      for( size_t i = 0; i < data.size(); ++i ) {
         m_values[ getBin( i ) ] = data[ i ];
      }
      m_time   = res.time;
      m_energy = res.energy;

      return true;
   }

   void RawHistogram::paintEvent( QPaintEvent* event ) {

      // First let the base class do the painting:
      moni::Histogram::paintEvent( event );

      // Determine the binning for the X axis:
      const AxisBinning xbin = getXAxisBinning();
      // Determine the binning for the Y axis:
      const AxisBinning ybin = getYAxisBinning();

      // Create a painter object for drawing the widget:
      QPainter painter( this );

      // Some basic values about the drawing area:
      const int x_axis_start = Y_AXIS_SPACING;
      const int y_axis_start = height() - X_AXIS_SPACING;
      const double bin_width = ( m_up - m_low ) / static_cast< double >( m_nbins );

      // Draw the CFD transformed distribution:
      if( m_trans.size() ) {
         painter.setPen( QPen( QBrush( QColor( Qt::darkMagenta ) ), 2 ) );
         int prev_pos = -1;
         for( size_t i = 0; i < m_trans.size() - 1; ++i ) {
            // Security check:
            if( ybin.getDrawPosition( m_transScale * m_trans[ i ] ) < 0.0 ) {
               prev_pos = y_axis_start;
               continue;
            }

            const int y_bin_pos =
               static_cast< int >( std::floor( y_axis_start -
                                               ybin.getDrawPosition( m_transScale *
                                                                     m_trans[ i ] ) ) );
            const int x_bin_low_pos =
               static_cast< int >( std::floor( xbin.getDrawPosition( m_low +
                                                                     i * bin_width ) ) );
            const int x_bin_up_pos =
               static_cast< int >( std::floor( xbin.getDrawPosition( m_low +
                                                                     ( i + 1 ) * bin_width ) ) );

            // Another security check:
            if( ( x_bin_low_pos < 0.0 ) || ( x_bin_up_pos < 0.0 ) ) continue;

            if( prev_pos > 0 ) {
               painter.drawLine( QLine( x_axis_start + x_bin_low_pos, prev_pos,
                                        x_axis_start + x_bin_low_pos, y_bin_pos ) );
            }
            prev_pos = y_bin_pos;
            painter.drawLine( QLine( x_axis_start + x_bin_low_pos, y_bin_pos,
                                     x_axis_start + x_bin_up_pos, y_bin_pos ) );
         }
      }

      // Draw the reconstructed energy:
      if( ybin.getDrawPosition( m_energy ) > 0.0 ) {
         const int energy_pos =
            static_cast< int >( std::floor( y_axis_start -
                                            ybin.getDrawPosition( m_energy ) ) );
         painter.setPen( QPen( QBrush( QColor( Qt::darkGreen ) ), 2 ) );
         painter.drawLine( QLine( x_axis_start, energy_pos,
                                  width() - 20, energy_pos ) );
      }

      // Draw the reconstructed time:
      if( xbin.getDrawPosition( m_time ) > 0.0 ) {
         const int time_pos =
            static_cast< int >( std::floor( x_axis_start +
                                            xbin.getDrawPosition( m_time ) ) );
         painter.setPen( QPen( QBrush( QColor( Qt::darkBlue ) ), 2 ) );
         painter.drawLine( QLine( time_pos, 20,
                                  time_pos, y_axis_start ) );
      }

      return;
   }

   std::pair< double, double >
   RawHistogram::getYAxisLimits() const {

      const double maximum =
         *( std::max_element( m_values.begin(), m_values.end() ) );
      const double tmaximum = ( m_trans.size() ?
                                *( std::max_element( m_trans.begin(), m_trans.end() ) ) :
                                0.0 );
      m_transScale = ( std::abs( tmaximum ) > 0.001 ?
                       0.8 * maximum / tmaximum : 1.0 );

      double view_maximum = 0.0;
      const double minimum =
         ( m_trans.size() ?
           std::min( *( std::min_element( m_values.begin(), m_values.end() ) ),
                     m_transScale * ( *std::min_element( m_trans.begin(), m_trans.end() ) ) ) :
           *( std::min_element( m_values.begin(), m_values.end() ) ) );
      double view_minimum = 0.0;
      if( m_yAxisStyle == Linear ) {
         if( maximum > 0.0 ) {
            view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 1.2 : 10.0 );
         } else {
            view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 0.8 : 10.0 );
         }
         if( minimum > 0.0 ) {
            view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 0.8 : 0.0 );
         } else {
            view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 1.2 : 0.0 );
         }
      } else if( m_yAxisStyle == Logarithmic ) {
         if( maximum > 0.0 ) {
            view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 5.0 : 10.0 );
         } else {
            view_maximum = 10.0;
         }
         if( minimum > 0.0 ) {
            view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 0.2 : 0.1 );
         } else {
            view_minimum = 0.1;
         }
      } else {
         REPORT_ERROR( tr( "Unknown Y axis style!" ) );
         view_maximum = maximum;
         view_minimum = minimum;
      }

      return std::make_pair( view_minimum, view_maximum );
   }

} // namespace dt5740
