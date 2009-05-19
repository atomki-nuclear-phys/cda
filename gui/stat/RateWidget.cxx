// $Id$

// STL include(s):
#include <algorithm>
#include <iostream>

// Qt include(s):
#include <QtCore/QPoint>
#include <QtCore/QLine>
#include <QtGui/QPainter>

// Local include(s):
#include "RateWidget.h"

namespace stat {

   const quint32 RateWidget::MARK_HEIGHT = 10;
   const quint32 RateWidget::RATE_HISTORY_LENGTH = 10;

   RateWidget::RateWidget( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ),
        m_rateValues( RATE_HISTORY_LENGTH, 0 ) {

      setMinimumSize( 120, 100 );
      setMaximumSize( 1200, 1000 );

   }

   RateWidget::~RateWidget() {

   }

   void RateWidget::setNewRate( double rate ) {

      m_rateValues.pop_front();
      m_rateValues.push_back( rate );
      update();

      return;

   }

   void RateWidget::paintEvent( QPaintEvent* /* event */ ) {

      QPainter painter( this );

      // Calculate the maximum rate, and the maximum on the Y axis:
      const double maximum =
         *( std::max_element( m_rateValues.begin(), m_rateValues.end() ) );
      const double view_maximum = ( maximum > 0.0 ? maximum : 10.0 );

      // Print the maximum rate:
      painter.setBrush( Qt::black );
      painter.drawText( QRect( 0, 0, width(), 20 ),
                        Qt::AlignCenter, tr( "Peak: " ) + QString::number( maximum ) +
                        " Hz" );

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

      // Draw the corners:
      painter.setPen( Qt::SolidLine );
      painter.drawLine( QLine( 0, 20, 5, 25 ) );
      painter.drawLine( QLine( width() - 5, height() - 5,
                               width(), height() ) );

      // Draw the centre area:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::black );
      painter.drawRect( QRect( 5, 25, width() - 10, height() - 30 ) );

      // Calculate the size of the marks:
      const int mark_width = ( width() - 10 ) / RATE_HISTORY_LENGTH;
      const int mark_height = ( height() - 30 ) / ( ( height() - 30 ) / MARK_HEIGHT );

      // Draw the marks themselves:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::green );
      int column = 0;
      for( std::list< double >::const_iterator it = m_rateValues.begin();
           it != m_rateValues.end(); ++it, ++column ) {
         const int marks = ( ( height() - 30 ) / mark_height ) * ( *it / view_maximum );
         for( int mark = 0; mark < marks; ++mark ) {
            const int rect_x = 5 + column * mark_width;
            const int rect_y = ( height() - 5 - mark_height ) - ( mark * mark_height );
            painter.drawRect( QRect( rect_x, rect_y,
                                     ( column == RATE_HISTORY_LENGTH - 1 ?
                                       width() - 5 - rect_x : mark_width ),
                                     mark_height ) );
         }
      }

      return;

   }

} // namespace stat
