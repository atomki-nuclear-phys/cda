// $Id$

// System include(s):
#include <cmath>

// Qt include(s):
#include <QtCore/QLine>
#include <QtGui/QPainter>

// Local include(s):
#include "RawHistogram.h"

namespace dt5740 {

   RawHistogram::RawHistogram( QWidget* parent, Qt::WindowFlags flags )
      : moni::Histogram( parent, flags ),
        m_time( 0.0 ), m_energy( 0.0 ) {

   }

   RawHistogram::RawHistogram( const QString& title, int bins, double low,
                               double up, int refreshTimeout,
                               QWidget* parent, Qt::WindowFlags flags )
      : moni::Histogram( title, bins, low, up, refreshTimeout,
                         parent, flags ),
        m_time( 0.0 ), m_energy( 0.0 ) {

   }

   void RawHistogram::setReconstructedTime( double value ) {

      m_time = value;
      return;
   }

   void RawHistogram::setReconstructedEnergy( double value ) {

      m_energy = value;
      return;
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

} // namespace dt5740
