// $Id$

// STL include(s):
#include <algorithm>
#include <cmath>

// Qt include(s):
#include <QtCore/QPoint>
#include <QtCore/QLine>
#include <QtGui/QPainter>

// Local include(s):
#include "RateWidget.h"

namespace cdastat {

   //
   // Specify the values of the constants:
   //
   const quint32 RateWidget::MARK_HEIGHT = 10;
   const quint32 RateWidget::RATE_HISTORY_LENGTH = 20;

   /**
    * The constructor initialises the member variable(s) and sets the minimum and
    * maximum allowed sizes of the widget.
    */
   RateWidget::RateWidget( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_rateValues( RATE_HISTORY_LENGTH, 0.0 ) {

      setMinimumSize( 120, 100 );
      setMaximumSize( 1200, 1000 );
   }

   /**
    * This function stores the new rate value, and throws away the oldest stored
    * rate value. Then it asks for the widget to be re-drawn.
    *
    * @param rate The rate value that should be shown
    */
   void RateWidget::setNewRate( qreal rate ) {

      m_rateValues.pop_front();
      m_rateValues.push_back( rate );
      update();

      return;
   }

   /**
    * This function is responsible for drawing the widget. It uses basic Qt drawing
    * techniques to do this.
    */
   void RateWidget::paintEvent( QPaintEvent* ) {

      // Create a painter object for drawing the widget:
      QPainter painter( this );

      // Calculate the maximum rate, and the maximum on the Y axis:
      const double maximum =
         *( std::max_element( m_rateValues.begin(), m_rateValues.end() ) );
      const double view_maximum = ( maximum > 0.0 ? maximum : 10.0 );

      // Print the maximum rate:
      painter.setBrush( Qt::black );
      painter.drawText( QRect( 0, 0, width(), 20 ),
                        Qt::AlignCenter, tr( "Peak: %1 Hz" ).arg( QString::number( maximum ) ) );

      // Draw the background rectangle:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::lightGray );
      painter.drawRect( QRect( 0, 20, width(), height() - 20 ) );

      // Draw the shadowy edge:
      const QPoint edge1[ 4 ] = {
         QPoint( 0, 20 ),
         QPoint( 0, height() ),
         QPoint( 5, height() - 5 ),
         QPoint( 5, 25 )
      };
      const QPoint edge2[ 4 ] = {
         QPoint( 0, 20 ),
         QPoint( 5, 25 ),
         QPoint( width() - 5, 25 ),
         QPoint( width(), 20 )
      };
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::darkGray );
      painter.drawPolygon( edge1, 4 );
      painter.drawPolygon( edge2, 4 );

      // Draw the corner lines:
      painter.setPen( Qt::SolidLine );
      painter.drawLine( QLine( 0, 20, 5, 25 ) );
      painter.drawLine( QLine( width() - 5, height() - 5,
                               width(), height() ) );

      // Draw the centre area:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::black );
      painter.drawRect( QRect( 5, 25, width() - 10, height() - 30 ) );

      // Calculate the size of the marks:
      const qreal mark_width = static_cast< qreal >( width() - 10.0 ) / RATE_HISTORY_LENGTH;
      const qreal mark_height = static_cast< qreal >( height() - 30.0 ) /
         static_cast< int >( ( height() - 30 ) / MARK_HEIGHT );

      // Draw the marks themselves:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::green );
      int column = 0;
      for( std::list< qreal >::const_iterator it = m_rateValues.begin();
           it != m_rateValues.end(); ++it, ++column ) {
         const int marks =
            static_cast< int >( std::floor( ( ( height() - 30 ) / mark_height ) *
                                            ( *it / view_maximum ) ) );
         for( int mark = 0; mark < marks; ++mark ) {
            const qreal rect_x = 5 + column * mark_width;
            const qreal rect_y = ( height() - 5 - mark_height ) - ( mark * mark_height );
            painter.drawRect( QRectF( rect_x, rect_y,
                                      ( column == RATE_HISTORY_LENGTH - 1 ?
                                        width() - 5 - rect_x : mark_width ),
                                      mark_height ) );
         }
      }

      return;
   }

} // namespace cdastat
