
// System include(s):
#include <algorithm>
#include <cmath>

// Qt include(s):
#include <QPoint>
#include <QLine>
#include <QRect>
#include <QPainter>
#include <QMouseEvent>
#include <QMenu>
#include <QActionGroup>

// Local include(s):
#include "Histogram.h"
#include "AxisStyleAction.h"

namespace moni {

   //
   // Set the values of all the constants:
   //
   const int Histogram::TICK_LENGTH_MAJOR = 15;
   const int Histogram::TICK_LENGTH_MINOR = 8;
   const int Histogram::MIN_X_TICK_DISTANCE = 80;
   const int Histogram::MIN_Y_TICK_DISTANCE = 40;

   const int Histogram::X_AXIS_SPACING = 60;
   const int Histogram::Y_AXIS_SPACING = 80;

   /**
    * Constructor mostly useful when the properties of the histogram are
    * not yet known at construction. The histogram specific members are
    * filled in with some defaults when using this constructor.
    *
    * @param parent The Qt widget parent of the histogram
    * @param flags Qt specific flags for the object
    */
   Histogram::Histogram( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_title( "N/A" ), m_nbins( 100 ),
        m_low( 0.0 ), m_up( 100.0 ), m_viewLow( 0.0 ), m_viewUp( 100.0 ),
        m_isZooming( false ), m_zoomStart( 0 ), m_zoomCurrent( 0 ),
        m_hColor( Qt::darkRed ),
        m_refreshTimer( this ),
        m_xAxisStyle( Linear ), m_yAxisStyle( Linear ),
        m_values( 102, 0.0 ), m_entries( 0 ),
        m_logger( "moni::Histogram" ) {

      setMinimumSize( 220, 150 );
      setMaximumSize( 2200, 2000 );

      reset();

      // Set up the timer for the regular updates:
      m_refreshTimer.setInterval( 2000 );
      m_refreshTimer.setSingleShot( false );
      connect( &m_refreshTimer, SIGNAL( timeout() ),
               this, SLOT( update() ) );
      m_refreshTimer.start();
   }

   /**
    * Constructor explicitly specifying all the histogram specific
    * properties of the created object. It will probably only be used
    * in the standalone examples, but who knows.
    *
    * @param title Title to be shown in the histogram's frame
    * @param bins The number of bins for the histogram
    * @param low The lower edge of the X axis
    * @param up  The upper edge of the X axis
    * @param refreshTimeout The time between two updates of the graphical object
    * @param parent The Qt widget parent of the histogram
    * @param flags Qt specific flags for the object
    */
   Histogram::Histogram( const QString& title, int bins, double low, double up,
                         int refreshTimeout,
                         QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_title( title ), m_nbins( bins ),
        m_low( low ), m_up( up ), m_viewLow( low ), m_viewUp( up ),
        m_isZooming( false ), m_zoomStart( 0 ), m_zoomCurrent( 0 ),
        m_hColor( Qt::darkRed ),
        m_refreshTimer( this ),
        m_xAxisStyle( Linear ), m_yAxisStyle( Linear ),
        m_values( static_cast< size_t >( bins + 2 ), 0.0 ), m_entries( 0 ),
        m_logger( "moni::Histogram" ) {

      setMinimumSize( 220, 150 );
      setMaximumSize( 2200, 2000 );

      reset();

      // Set up the timer for the regular updates:
      m_refreshTimer.setInterval( refreshTimeout );
      m_refreshTimer.setSingleShot( false );
      connect( &m_refreshTimer, SIGNAL( timeout() ),
               this, SLOT( update() ) );
      m_refreshTimer.start();
   }

   const QString& Histogram::getTitle() const {

      return m_title;
   }

   int Histogram::getNBins() const {

      return m_nbins;
   }

   double Histogram::getLowerBound() const {

      return m_low;
   }

   double Histogram::getUpperBound() const {

      return m_up;
   }

   int Histogram::getRefreshTimeout() const {

      return m_refreshTimer.interval();
   }

   void Histogram::setRefreshTimeout( int value ) {

      m_refreshTimer.stop();
      m_refreshTimer.setInterval( value );
      m_refreshTimer.start();
      return;
   }

   Histogram::AxisStyle Histogram::getXAxisStyle() const {

      return m_xAxisStyle;
   }

   Histogram::AxisStyle Histogram::getYAxisStyle() const {

      return m_yAxisStyle;
   }

   void Histogram::setTitle( const QString& title ) {

      m_title = title;
      update();
      return;
   }

   void Histogram::setNBins( int bins ) {

      m_nbins = bins;
      reset();
      return;
   }

   void Histogram::setLowerBound( double low ) {

      m_low = low;
      m_viewLow = low;
      reset();
      return;
   }

   void Histogram::setUpperBound( double up ) {

      m_up = up;
      m_viewUp = up;
      reset();
      return;
   }

   void Histogram::setHistColor( const QColor& color ) {

      m_hColor = color;
      update();
      return;
   }

   void Histogram::setXAxisStyle( AxisStyle style ) {

      m_xAxisStyle = style;
      update();
      return;
   }

   void Histogram::setYAxisStyle( AxisStyle style ) {

      m_yAxisStyle = style;
      update();
      return;
   }

   void Histogram::unzoom() {

      // Reset the view options:
      m_viewLow = m_low;
      m_viewUp = m_up;

      update();
      return;
   }

   /**
    * This function clears the contents of the histogram, updating
    * its internal cache to be in sync with the current configuration.
    * It also performs a simple check on the X axis limits.
    */
   void Histogram::reset() {

      m_values.clear();
      m_values.resize( static_cast< size_t >( m_nbins + 2 ), 0.0 );
      m_entries = 0;
      m_viewLow = m_low;
      m_viewUp = m_up;

      // Check if the configured values make sense:
      if( ! ( m_up > m_low ) ) {
         REPORT_ERROR( tr( "The upper bound of histogram '%1' "
                           "is not larger than its lower bound" )
                       .arg( m_title ) );
      }

      update();
      return;
   }

   /**
    * Function used to fill the histogram. Just like in ROOT, it is
    * possible to fill entries using a weight, but in the monitoring this
    * is probably not going to be used. (I was wrong on the weights...)
    *
    * @param value The value at which the histogram should be filled
    * @param weight The weight with which the histogram should be filled
    */
   void Histogram::fill( double value, double weight ) {

      // Check if the given parameters make sense:
      if( std::isnan( value ) || std::isnan( weight ) ) {
         REPORT_ERROR( tr( "fill( value = %1, weight = %2 ): NaN received" )
                       .arg( value ).arg( weight ) );
         return;
      }

      // Fill the histogram:
      m_values[ getBin( value ) ] += weight;
      ++m_entries;

      return;
   }

   /**
    * This function is called by Qt when the widget needs to re-paint itself.
    * (After resizing the window, or something similar.) It takes care of
    * painting the histogram using the current configuration.
    */
   void Histogram::paintEvent( QPaintEvent* ) {

      // Create a painter object for drawing the widget:
      QPainter painter( this );

      // Draw the white background:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::white );
      painter.drawRect( QRect( 0, 0, width() - 1, height() - 1 ) );

      // Draw the rectangle of the histogram:
      painter.drawRect( QRect( Y_AXIS_SPACING, 20,
                               width() - ( Y_AXIS_SPACING + 20 ),
                               height() - ( X_AXIS_SPACING + 20 ) ) );

      // Draw the different parts of the historgam:
      drawXAxis( painter );
      drawYAxis( painter );
      drawHist( painter );
      drawZoom( painter );
      drawStat( painter );

      // Finally, show the title of the histogram:
      painter.setPen( Qt::SolidLine );
      painter.drawText( QRect( 0, height() - X_AXIS_SPACING + 35,
                               width(), 25 ),
                        Qt::AlignCenter, m_title );

      return;
   }

   void Histogram::mousePressEvent( QMouseEvent* event ) {

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
      const qreal event_x = event->x();
      const qreal event_y = event->y();
#else
      const qreal event_x = event->position().x();
      const qreal event_y = event->position().y();
#endif

      // If it's the left button, enter zooming mode:
      if( event->button() == Qt::LeftButton ) {
         // Check that the cursor is around the X axis:
         if( ( event_y < ( height() - X_AXIS_SPACING - 20 ) ) ||
             ( event_y > ( height() - X_AXIS_SPACING + 20 ) ) ||
             ( event_x < Y_AXIS_SPACING ) ||
             ( event_x > width() - 20 ) ) {
            return;
         }
         // Check that the X axis is drawn in linear mode:
         if( m_xAxisStyle != Linear ) {
            return;
         }
         // Enter zooming mode:
         m_isZooming = true;
         m_zoomStart = event_x - Y_AXIS_SPACING;
         m_zoomCurrent = m_zoomStart;
         update();
         return;
      }

      // Return if it's not a right click:
      if( event->button() != Qt::RightButton ) {
         return;
      }

      // Create the pup-up menu:
      QMenu* menu = new QMenu( this );

      // Sub-menu for changing the X axis style:
      QMenu* xAxisMenu = menu->addMenu( tr( "Set X axis style" ) );
      AxisStyleAction* xLinAction = new AxisStyleAction( Linear, tr( "Linear" ),
                                                         xAxisMenu );
      connect( xLinAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
               this, SLOT( setXAxisStyle( Histogram::AxisStyle ) ) );
      // Only offer the logarithmic axis style if it makes sense:
      AxisStyleAction* xLogAction = 0;
      if( getLogXPossible() ) {
         xLogAction = new AxisStyleAction( Logarithmic,
                                           tr( "Logarithmic" ),
                                           xAxisMenu );
         connect( xLogAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
                  this, SLOT( setXAxisStyle( Histogram::AxisStyle ) ) );
      }

      // Group these actions together:
      QActionGroup* xAxisStyleGroup = new QActionGroup( xAxisMenu );
      xAxisStyleGroup->addAction( xLinAction );
      if( xLogAction ) {
         xAxisStyleGroup->addAction( xLogAction );
      }
      if( getXAxisStyle() == Linear ) {
         xLinAction->setChecked( true );
      } else if( ( getXAxisStyle() == Logarithmic ) && xLogAction ) {
         xLogAction->setChecked( true );
      }

      // Add the two actions to the sub-menu:
      xAxisMenu->addAction( xLinAction );
      if( xLogAction ) {
         xAxisMenu->addAction( xLogAction );
      }

      // Sub-menu for changing the Y axis style:
      QMenu* yAxisMenu = menu->addMenu( tr( "Set Y axis style" ) );
      AxisStyleAction* yLinAction = new AxisStyleAction( Linear, tr( "Linear" ),
                                                         yAxisMenu );
      connect( yLinAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
               this, SLOT( setYAxisStyle( Histogram::AxisStyle ) ) );
      // Only offer the logarithmic axis style if it makes sense:
      AxisStyleAction* yLogAction = 0;
      if( getLogYPossible() ) {
         yLogAction = new AxisStyleAction( Logarithmic,
                                           tr( "Logarithmic" ),
                                           yAxisMenu );
         connect( yLogAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
                  this, SLOT( setYAxisStyle( Histogram::AxisStyle ) ) );
      }

      // Group these actions together:
      QActionGroup* yAxisStyleGroup = new QActionGroup( yAxisMenu );
      yAxisStyleGroup->addAction( yLinAction );
      if( yLogAction ) {
         yAxisStyleGroup->addAction( yLogAction );
      }
      if( getYAxisStyle() == Linear ) {
         yLinAction->setChecked( true );
      } else if( ( getYAxisStyle() == Logarithmic ) && yLogAction ) {
         yLogAction->setChecked( true );
      }

      // Add the two actions to the sub-menu:
      yAxisMenu->addAction( yLinAction );
      if( yLogAction ) {
         yAxisMenu->addAction( yLogAction );
      }

      // Action canceling the zoom on the X axis:
      QAction* unzoomAction = menu->addAction( tr( "Un-zoom" ) );
      connect( unzoomAction, SIGNAL( triggered() ),
               this, SLOT( unzoom() ) );

      // Action clearing the histogram:
      QAction* resetAction = menu->addAction( tr( "Clear histogram" ) );
      connect( resetAction, SIGNAL( triggered() ),
               this, SLOT( reset() ) );

      // Display the menu under the cursor:
      menu->exec( mapToGlobal( QPoint( event_x, event_y ) ) );

      return;
   }

   void Histogram::mouseReleaseEvent( QMouseEvent* event ) {

      // Check that we are in zooming mode:
      if( ! m_isZooming ) {
         return;
      }

      // Only handle the left mouse button:
      if( event->button() != Qt::LeftButton ) {
         return;
      }

      // Check that the X axis is drawn in linear mode:
      if( m_xAxisStyle != Linear ) {
         return;
      }

      // Leave zooming mode:
      m_isZooming = false;

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
      const qreal event_x = event->x();
      const qreal event_y = event->y();
#else
      const qreal event_x = event->position().x();
      const qreal event_y = event->position().y();
#endif

      // Decide on the final point of the zoom:
      if( event_x > ( width() - 20 ) ) {
         m_zoomCurrent = width() - 20;
      } else if( event_x < Y_AXIS_SPACING ) {
         m_zoomCurrent = Y_AXIS_SPACING;
      } else {
         m_zoomCurrent = event_x - Y_AXIS_SPACING;
      }

      // Check that the zoom distance makes sense:
      if( std::abs( m_zoomCurrent - m_zoomStart ) < 5 ) {
         return;
      }

      // Get the X axis binning:
      const AxisBinning abin = getXAxisBinning();

      // Translate the zoom positions into values on the axis:
      const double tempLow = abin.getValue( m_zoomStart );
      const double tempUp  = abin.getValue( m_zoomCurrent );

      // Now update the view limits:
      if( tempUp > tempLow ) {
         m_viewLow = tempLow;
         m_viewUp  = tempUp;
      } else {
         m_viewLow = tempUp;
         m_viewUp  = tempLow;
      }

      // Draw the histogram:
      update();

      return;
   }

   void Histogram::mouseMoveEvent( QMouseEvent* event ) {

      // Check that we are in zooming mode:
      if( ! m_isZooming ) {
         return;
      }

      // Check that the X axis is drawn in linear mode:
      if( m_xAxisStyle != Linear ) {
         return;
      }

#if QT_VERSION < QT_VERSION_CHECK( 6, 0, 0 )
      const qreal event_x = event->x();
      const qreal event_y = event->y();
#else
      const qreal event_x = event->position().x();
      const qreal event_y = event->position().y();
#endif

      // Decide on the final point of the zoom:
      if( event_x > ( width() - 20 ) ) {
         m_zoomCurrent = width() - 20;
      } else if( event_x < Y_AXIS_SPACING ) {
         m_zoomCurrent = Y_AXIS_SPACING;
      } else {
         m_zoomCurrent = event_x - Y_AXIS_SPACING;
      }

      update();
      return;
   }

   /**
    * A very simple function for determining which bin represents a given
    * value.
    *
    * @param value A value on the X axis
    * @returns The bin representing the specified value
    */
   size_t Histogram::getBin( double value ) const {

      if( value < m_low ) return 0;
      if( value > m_up ) return static_cast< size_t >( m_nbins + 1 );

      return static_cast< size_t >( std::floor( ( value - m_low ) /
                                                ( ( m_up - m_low ) / m_nbins ) )
                                                + 1 );
   }

   /**
    * This function draws the X axis of the histogram. It just nicely labels the
    * axis depending on the settings of the histogram.
    *
    * @param painter The painter object to use for the drawing
    */
   void Histogram::drawXAxis( QPainter& painter ) const {

      // Some basic value(s) about the axis:
      const int axis_start = Y_AXIS_SPACING;

      // Determine the binning for the axis:
      const AxisBinning abin = getXAxisBinning();

      // Decide whether to print the axis values with exponents:
      const std::pair< bool, int > exp = getExponent( abin );

      // Draw the "major ticks" together with the labels:
      std::vector< AxisBinning::MajorTick >::const_iterator major_itr =
         abin.majors().begin();
      std::vector< AxisBinning::MajorTick >::const_iterator major_end =
         abin.majors().end();
      for( ; major_itr != major_end; ++major_itr ) {

         // Calculate the position of the tick:
         const int tick_location =
            static_cast< int >( std::floor( axis_start +
                                            major_itr->position() ) );
         if( ( tick_location < axis_start ) ||
             ( tick_location > ( width() - 20 ) ) ) continue;

         // Draw the line of the tick:
         painter.setPen( Qt::SolidLine );
         painter.drawLine( QLine( tick_location, height() - X_AXIS_SPACING,
                                  tick_location, height() - X_AXIS_SPACING +
                                  TICK_LENGTH_MAJOR ) );

         // Construct the proper tick value:
         double tick_value = major_itr->value();
         if( exp.first ) {
            tick_value /= std::pow( 10.0, exp.second );
         }
         // Now draw it on the axis:
         painter.drawText( QRect( tick_location - MIN_X_TICK_DISTANCE / 2,
                                  height() - ( X_AXIS_SPACING - 20 ),
                                  MIN_X_TICK_DISTANCE, 20 ),
                           Qt::AlignCenter, QString::number( tick_value ) );

         // Draw the dotted line across the pad:
         painter.setPen( Qt::DotLine );
         painter.drawLine( QLine( tick_location, height() - X_AXIS_SPACING,
                                  tick_location, 20 ) );
      }

      // If exponents were used, put that beside the axis:
      if( exp.first ) {
         painter.drawText( QRect( width() - 70, height() - X_AXIS_SPACING + 35,
                                  50, 20 ),
                           Qt::AlignCenter,
                           // This should be painted much nicer once I'll have time:
                           QString( "x10^%1" ).arg( exp.second ) );
      }

      // Draw the "minor ticks":
      painter.setPen( Qt::SolidLine );
      std::vector< AxisBinning::MinorTick >::const_iterator minor_itr =
         abin.minors().begin();
      std::vector< AxisBinning::MinorTick >::const_iterator minor_end =
         abin.minors().end();
      for( ; minor_itr != minor_end; ++minor_itr ) {
         const int tick_location =
            static_cast< int >( std::floor( axis_start +
                                            minor_itr->position() ) );
         if( ( tick_location < axis_start ) ||
             ( tick_location > ( width() - 20 ) ) ) continue;
         painter.drawLine( QLine( tick_location, height() - X_AXIS_SPACING,
                                  tick_location, height() - X_AXIS_SPACING +
                                  TICK_LENGTH_MINOR ) );
      }

      return;
   }

   /**
    * This function draws the Y axis of the histogram. It just nicely labels the
    * axis depending on the contents of the histogram.
    *
    * @param painter The painter object to use for the drawing
    */
   void Histogram::drawYAxis( QPainter& painter ) const {

      // Some basic value(s) about the axis:
      const int axis_start = height() - X_AXIS_SPACING;

      // Determine the binning for the axis:
      const AxisBinning abin = getYAxisBinning();

      // Decide whether to print the axis values with exponents:
      const std::pair< bool, int > exp = getExponent( abin );

      // Draw the "major ticks" together with the labels:
      std::vector< AxisBinning::MajorTick >::const_iterator major_itr =
         abin.majors().begin();
      std::vector< AxisBinning::MajorTick >::const_iterator major_end =
         abin.majors().end();
      for( ; major_itr != major_end; ++major_itr ) {

         // Calculate the position of the tick:
         const int tick_location =
            static_cast< int >( std::floor( axis_start -
                                            major_itr->position() ) );
         if( ( tick_location < 20 ) ||
             ( tick_location > axis_start ) ) continue;

         // Draw the line of the tick:
         painter.setPen( Qt::SolidLine );
         painter.drawLine( QLine( Y_AXIS_SPACING - TICK_LENGTH_MAJOR,
                                  tick_location,
                                  Y_AXIS_SPACING, tick_location ) );

         // Construct the proper tick value:
         double tick_value = major_itr->value();
         if( exp.first ) {
            tick_value /= std::pow( 10.0, exp.second );
         }
         // Now draw it on the axis:
         painter.drawText( QRect( 5, tick_location - MIN_Y_TICK_DISTANCE / 2,
                                  Y_AXIS_SPACING - TICK_LENGTH_MAJOR - 3,
                                  MIN_Y_TICK_DISTANCE ),
                           Qt::AlignCenter, QString::number( tick_value ) );

         // Draw the dotted line across the pad:
         painter.setPen( Qt::DotLine );
         painter.drawLine( QLine( Y_AXIS_SPACING, tick_location,
                                  width() - 20, tick_location ) );
      }

      // If exponents were used, put that beside the axis:
      if( exp.first ) {
         painter.drawText( QRect( 5, 5, 50, 20 ),
                           Qt::AlignCenter,
                           // This should be painted much nicer once I'll have time:
                           QString( "x10^%1" ).arg( exp.second ) );
      }

      // Draw the "minor ticks":
      painter.setPen( Qt::SolidLine );
      std::vector< AxisBinning::MinorTick >::const_iterator minor_itr =
         abin.minors().begin();
      std::vector< AxisBinning::MinorTick >::const_iterator minor_end =
         abin.minors().end();
      for( ; minor_itr != minor_end; ++minor_itr ) {
         const int tick_location =
            static_cast< int >( std::floor( axis_start -
                                            minor_itr->position() ) );
         if( ( tick_location < 20 ) ||
             ( tick_location > axis_start ) ) continue;
         painter.drawLine( QLine( Y_AXIS_SPACING - TICK_LENGTH_MINOR,
                                  tick_location,
                                  Y_AXIS_SPACING, tick_location ) );
      }

      return;
   }

   /**
    * This function actually draws the histogram. It draws it in a fashion
    * very similar to when ROOT needs to draw a histogram with the "HIST"
    * option. But it also fills the histogram using a cross pattern as well.
    *
    * @param painter The painter object to use for the drawing
    */
   void Histogram::drawHist( QPainter& painter ) const {

      //
      // Some basic values about the drawing area:
      //
      const int x_axis_start = Y_AXIS_SPACING;
      const int y_axis_start = height() - X_AXIS_SPACING;
      const double bin_width = ( ( m_up - m_low ) /
                                 static_cast< double >( m_nbins ) );

      // Determine the binning for the X axis:
      const AxisBinning xbin = getXAxisBinning();
      // Determine the binning for the Y axis:
      const AxisBinning ybin = getYAxisBinning();

      // Set the histogram color for the painter:
      painter.setPen( QPen( QBrush( m_hColor ), 2 ) );

      // Draw the histogram:
      int prev_pos = -1;
      for( int i = 0; i < m_nbins; ++i ) {

         const size_t ihelp = static_cast< size_t >( i + 1 );

         const int y_bin_pos =
            ( ybin.getDrawPosition( m_values[ ihelp ] ) > 0.0 ?
              static_cast< int >( std::floor( y_axis_start -
                                              ybin.getDrawPosition( m_values[ ihelp ] ) ) ) :
              y_axis_start );
         const int x_bin_low_pos =
            static_cast< int >( std::floor( xbin.getDrawPosition( m_low +
                                                                  i * bin_width ) ) );
         const int x_bin_up_pos =
            static_cast< int >( std::floor( xbin.getDrawPosition( m_low +
                                                                  ( i + 1 ) * bin_width ) ) );

         // A security check:
         if( ( x_bin_low_pos < 0.0 ) || ( x_bin_up_pos < 0.0 ) ) continue;

         if( prev_pos > 0 ) {
            painter.drawLine( QLine( x_axis_start + x_bin_low_pos, prev_pos,
                                     x_axis_start + x_bin_low_pos, y_bin_pos ) );
         }
         prev_pos = y_bin_pos;
         painter.drawLine( QLine( x_axis_start + x_bin_low_pos, y_bin_pos,
                                  x_axis_start + x_bin_up_pos, y_bin_pos ) );
      }

      // Reset the painter settings:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::white );

      return;
   }

   void Histogram::drawZoom( QPainter& painter ) const {

      // Check that we are in zooming mode:
      if( ! m_isZooming ) {
         return;
      }

      // Draw a line at the start of the zoom:
      painter.setPen( QPen( QBrush( Qt::blue ), 1 ) );
      painter.drawLine( QLine( Y_AXIS_SPACING + m_zoomStart, 20,
                               Y_AXIS_SPACING + m_zoomStart,
                               height() - X_AXIS_SPACING ) );

      // Draw a line at the current location of the zoom:
      painter.setPen( QPen( QBrush( Qt::red ), 1 ) );
      painter.drawLine( QLine( Y_AXIS_SPACING + m_zoomCurrent, 20,
                               Y_AXIS_SPACING + m_zoomCurrent,
                               height() - X_AXIS_SPACING ) );

      // Reset the painter settings:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::white );

      return;
   }

   /**
    * This function is used to draw some minimal statistics about the histogram
    * into the frame when the widget is large enough for it.
    *
    * @param painter The painter object to use for the drawing
    */
   void Histogram::drawStat( QPainter& painter ) const {

      // Return right away if the widget is not large enough to hold the
      // statistics information:
      if( ( width() < 400 ) || ( height() < 150 ) ) {
         return;
      }

      // Draw the frame of the statistics info:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::white );
      painter.drawRect( QRect( width() - 170, 20, 150, 60 ) );

      // Draw the statistics as simple text:
      painter.drawText( QRect( width() - 160, 20, 130, 60 ),
                        Qt::AlignLeft | Qt::AlignVCenter,
                        tr( "Entries: %1\n"
                            "Underflows: %2\n"
                            "Overflows: %3" ).arg( m_entries )
                        .arg( m_values.front() ).arg( m_values.back() ) );

      return;
   }

   /**
    * This function figures out how to draw the labels onto an axis with a
    * linear layout. The returned structure describes how the axis should be
    * drawn.
    *
    * @param low The lower edge on the axis
    * @param up  The upper edge on the axis
    * @param alength The length of the axis in pixels
    * @param tdist Minimal distance between the major ticks
    * @returns A structure describing the proposed axis decoration
    */
   Histogram::AxisBinning
   Histogram::getLinearAxisBinning( double low, double up,
                                    double alength, int tdist ) const {

      // The unit length in pixels:
      const double axis_unit = alength / ( up - low );

      //
      // The major tick distance in the axis units:
      //
      double tick_major_unit = std::pow( 10.0,
                                         std::ceil( std::log10( up - low ) ) -
                                         1.0 );
      while( tick_major_unit * axis_unit < tdist ) {
         tick_major_unit *= 2.0;
      }
      while( ( tick_major_unit * axis_unit / 2 ) > tdist ) {
         tick_major_unit /= 2.0;
      }

      //
      // The minor tick distance in axis units:
      //
      const double tick_minor_unit = tick_major_unit / 10.0;

      //
      // Units for the drawing:
      //
      const double tick_major_draw_unit = tick_major_unit * axis_unit;
      const double tick_minor_draw_unit = tick_minor_unit * axis_unit;

      //
      // Number of ticks for the axis:
      //
      const int n_major_ticks =
         static_cast< int >( std::ceil( ( up - low ) / tick_major_unit ) +
                             1.0 );
      const int n_minor_ticks =
         static_cast< int >( std::floor( ( up - low ) / tick_minor_unit ) +
                             1.0 );

      //
      // Offsets of the first major and minor tick marks:
      //
      const double tick_major_offset = ( std::fmod( -1.0 * low,
                                                    tick_major_unit ) *
                                         axis_unit );
      const double tick_minor_offset = ( std::fmod( -1.0 * low,
                                                    tick_minor_unit ) *
                                         axis_unit );

      //
      // The value associated with the first major tick mark:
      //
      double first_major_tick_value = 0.0;
      if( std::abs( low ) < 0.001 ) {
         first_major_tick_value = 0.0;
      } else {
         if( low < 0.0 ) {
            first_major_tick_value = ( std::ceil( low / tick_major_unit ) *
                                       tick_major_unit );
         } else {
            first_major_tick_value = ( std::floor( low / tick_major_unit ) *
                                       tick_major_unit );
         }
      }

      // Create the result structure:
      AxisBinning result( Linear, low, up, alength );

      // ...and now fill it:
      for( int i = 0; i < n_major_ticks; ++i ) {
         const double position = tick_major_offset + i * tick_major_draw_unit;
         if( ( position < 0.0 ) || ( position > alength ) ) continue;
         result.addMajorTick( AxisBinning::MajorTick( position,
                                                      first_major_tick_value +
                                                      i * tick_major_unit ) );
      }
      for( int i = 0; i < n_minor_ticks; ++i ) {
         const double position = tick_minor_offset + i * tick_minor_draw_unit;
         if( ( position < 0.0 ) || ( position > alength ) ) continue;
         result.addMinorTick( AxisBinning::MinorTick( position ) );
      }

      return result;
   }

   /**
    * This function figures out how to draw the labels onto an axis with a
    * logarithmic layout. The returned structure describes how the axis should
    * be drawn.
    *
    * @param low The lower edge on the axis
    * @param up  The upper edge on the axis
    * @param alength The length of the axis in pixels
    * @param tdist Minimal distance between the major ticks
    * @returns A structure describing the proposed axis decoration
    */
   Histogram::AxisBinning
   Histogram::getLogarithmicAxisBinning( double low, double up,
                                         double alength, int tdist ) const {

      // Check if this binning is possible:
      if( ( ! ( low > 0.0 ) ) || ( up < low ) ) {
         REPORT_ERROR( tr( "Unable to use logarithmic style with limits [%1,%2]" )
                       .arg( low ).arg( up ) );
         AxisBinning result( Logarithmic, low, up, alength );
         return result;
      }

      // Transform the axis units to logarithmic scale:
      const double low_log = std::log10( low );
      const double up_log  = std::log10( up );

      // The unit length in pixels:
      const double axis_unit = alength / ( up_log - low_log );

      //
      // The major tick distance in the axis units:
      //
      double tick_major_exponent_unit = 1.0;
      while( tick_major_exponent_unit * axis_unit < tdist ) {
         tick_major_exponent_unit += 1.0;
      }

      //
      // The value associated with the first major tick mark:
      //
      const double first_major_tick_value =
         std::ceil( std::fmod( std::abs( low_log ),
                               tick_major_exponent_unit ) + low_log );

      // Create the result structure:
      AxisBinning result( Logarithmic, low, up, alength );

      // ...and now fill it:
      int exponent = static_cast< int >( std::floor( first_major_tick_value -
                                                     tick_major_exponent_unit +
                                                     0.5 ) );
      double value = std::pow( 10.0, exponent );
      while( value < up ) {
         const double mvalue = std::pow( 10.0, exponent );
         if( ( mvalue > low ) && ( mvalue < up ) &&
             ( ! ( exponent % static_cast< int >( tick_major_exponent_unit ) ) ) ) {
            result.addMajorTick( AxisBinning::MajorTick( result.getDrawPosition( mvalue ),
                                                         mvalue ) );
         }
         for( int i = 1; i < 10; ++i ) {
            value = i * std::pow( 10.0, exponent );
            if( value > up ) break;
            if( value < low ) continue;
            result.addMinorTick( AxisBinning::MinorTick( result.getDrawPosition( value ) ) );
         }
         ++exponent;
      }

      return result;
   }

   /**
    * This function is used internally to figure out the minimum and maximum
    * values for the Y axis. The return value's first element stores the minimum
    * value on the Y axis, while the second element stores the maximum value.
    *
    * @returns The limits for the Y axis
    */
   std::pair< double, double > Histogram::getYAxisLimits() const {

      // Get the first and final bins that are shown:
      const std::vector< double >::difference_type start =
         static_cast< std::vector< double >::difference_type >( getBin( m_viewLow ) );
      const std::vector< double >::difference_type end =
         static_cast< std::vector< double >::difference_type >( getBin( m_viewUp ) );

      // Get the maximum and minimum values within the limits:
      const double maximum =
         *( std::max_element( ( m_values.begin() + start ),
                              ( m_values.begin() + end ) ) );
      double view_maximum = 0.0;
      const double minimum =
         *( std::min_element( ( m_values.begin() + start ),
                              ( m_values.begin() + end ) ) );
      double view_minimum = 0.0;

      // Choose smart limits for the Y axis, so that the histogram can be seen
      // nicely:
      if( m_yAxisStyle == Linear ) {
         if( maximum > 0.0 ) {
            view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 1.1 : 10.0 );
         } else {
            view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 0.9 : 10.0 );
         }
         if( minimum > 0.0 ) {
            view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 0.9 : 0.0 );
         } else {
            view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 1.1 : 0.0 );
         }
      } else if( m_yAxisStyle == Logarithmic ) {
         if( maximum > 0.0 ) {
            view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 2.0 : 10.0 );
         } else {
            view_maximum = 10.0;
         }
         if( minimum > 0.0 ) {
            view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 0.5 : 0.1 );
         } else {
            view_minimum = 0.1;
         }
      } else {
         REPORT_ERROR( tr( "Unknown Y axis style!" ) );
         view_maximum = maximum;
         view_minimum = minimum;
      }

      // Return the calculated numbers:
      return std::make_pair( view_minimum, view_maximum );
   }

   bool Histogram::getLogXPossible() const {

      return ( ( m_low > 0.0 ) && ( m_up > m_low ) );
   }

   bool Histogram::getLogYPossible() const {

      // Get the minimum to be shown in the plot:
      const double minimum =
         *( std::min_element( m_values.begin(), m_values.end() ) );

      // Only negative values exclude using logarithmic style for the Y axis:
      if( minimum < 0.0 ) {
         return false;
      } else {
         return true;
      }
   }

   Histogram::AxisBinning Histogram::getXAxisBinning() const {

      // Length of the X axis:
      const int axis_length = width() - ( Y_AXIS_SPACING + 20 );

      // Decide upon the correct axis binning:
      if( m_xAxisStyle == Linear ) {
         return getLinearAxisBinning( m_viewLow, m_viewUp, axis_length,
                                      MIN_X_TICK_DISTANCE );
      } else if( m_xAxisStyle == Logarithmic ) {
         return getLogarithmicAxisBinning( m_low, m_up, axis_length,
                                           MIN_X_TICK_DISTANCE );
      }

      // Handle programming errors gracefully:
      REPORT_ERROR( tr( "Binning style for the X axis (%1) "
                        "not understood, using linear binning" )
                    .arg( m_xAxisStyle ) );
      return getLinearAxisBinning( m_viewLow, m_viewUp, axis_length,
                                   MIN_X_TICK_DISTANCE );
   }

   Histogram::AxisBinning Histogram::getYAxisBinning() const {

      // Get the limits for the Y axis:
      const std::pair< double, double > limits = getYAxisLimits();
      // Length of the Y axis:
      const int axis_length = height() - ( X_AXIS_SPACING + 20 );

      // Decide upon the correct axis binning:
      if( m_yAxisStyle == Linear ) {
         return getLinearAxisBinning( limits.first, limits.second,
                                      axis_length, MIN_Y_TICK_DISTANCE );
      } else if( m_yAxisStyle == Logarithmic ) {
         return getLogarithmicAxisBinning( limits.first, limits.second,
                                           axis_length, MIN_Y_TICK_DISTANCE );
      }

      // Handle programming errors gracefully:
      REPORT_ERROR( tr( "Binning style for the Y axis (%1) "
                        "not understood, using linear binning" )
                    .arg( m_yAxisStyle ) );
      return getLinearAxisBinning( limits.first, limits.second,
                                   axis_length, MIN_Y_TICK_DISTANCE );
   }

   std::pair< bool, int >
   Histogram::getExponent( const Histogram::AxisBinning& abin ) const {

      if( abin.getStyle() == Linear ) {
         if( ( abin.majors().front().value() < -1000.0 ) ||
             ( abin.majors().back().value() > 1000.0 ) ) {
            // Decide on the exponent based on the largest major tick:
            const int exponent =
               static_cast< int >( std::floor( std::log10( std::max( std::abs( abin.majors().front().value() ),
                                                                     std::abs( abin.majors().back().value() ) ) ) ) );
            return std::make_pair( true, exponent );
         } else {
            return std::make_pair( false, -1 );
         }
      } else if( abin.getStyle() == Logarithmic ) {
         if( abin.majors().back().value() > 1000.0 ) {
            // Take an average of the exponents:
            int exponent = 0;
            std::vector< AxisBinning::MajorTick >::const_iterator itr =
               abin.majors().begin();
            std::vector< AxisBinning::MajorTick >::const_iterator end =
               abin.majors().end();
            for( ; itr != end; ++itr ) {
               exponent +=
                  static_cast< int >( std::floor( std::log10( itr->value() ) ) );
            }
            exponent /= static_cast< int >( abin.majors().size() );
            return std::make_pair( true, exponent );
         } else {
            return std::make_pair( false, -1 );
         }
      } else {
         REPORT_ERROR( tr( "Unknown axis style encountered" ) );
         return std::make_pair( false, -1 );
      }
   }

   Histogram::AxisBinning::MajorTick::MajorTick( tick_position position,
                                                 tick_value value )
      : m_position( position ), m_value( value ) {

   }

   Histogram::AxisBinning::tick_position
   Histogram::AxisBinning::MajorTick::position() const {

      return m_position;
   }

   Histogram::AxisBinning::tick_value
   Histogram::AxisBinning::MajorTick::value() const {

      return m_value;
   }

   Histogram::AxisBinning::MinorTick::MinorTick( tick_position position )
      : m_position( position ) {

   }

   Histogram::AxisBinning::tick_position
   Histogram::AxisBinning::MinorTick::position() const {

      return m_position;
   }

   Histogram::AxisBinning::AxisBinning( Histogram::AxisStyle style, double low,
                                        double up, double alength )
      : m_majors(), m_minors(), m_style( style ), m_low( low ), m_up( up ),
        m_alength( alength ) {

   }

   void Histogram::AxisBinning::
   addMajorTick( const Histogram::AxisBinning::MajorTick& tick ) {

      m_majors.push_back( tick );
      return;
   }

   void Histogram::AxisBinning::
   addMinorTick( const Histogram::AxisBinning::MinorTick& tick ) {

      m_minors.push_back( tick );
      return;
   }

   const std::vector< Histogram::AxisBinning::MajorTick >&
   Histogram::AxisBinning::majors() const {

      return m_majors;
   }

   const std::vector< Histogram::AxisBinning::MinorTick >&
   Histogram::AxisBinning::minors() const {

      return m_minors;
   }

   Histogram::AxisStyle Histogram::AxisBinning::getStyle() const {

      return m_style;
   }

   double Histogram::AxisBinning::getDrawPosition( double pos ) const {

      if( ( pos < ( m_low - 0.001 ) ) || ( pos > ( m_up + 0.001 ) ) ) {
         return -1.0;
      }

      if( m_style == Linear ) {
         return ( pos - m_low ) / ( m_up - m_low ) * m_alength;
      } else if( m_style == Logarithmic ) {
         if( ( m_low < 0.0 ) || ( pos < 0.0 ) ) return -1.0;
         return ( std::log10( pos ) - std::log10( m_low ) ) /
            ( std::log10( m_up ) - std::log10( m_low ) ) * m_alength;
      } else {
         return -1.0;
      }
   }

   double Histogram::AxisBinning::getValue( double drawPos ) const {

      if( ( drawPos < 0.001 ) || ( drawPos > ( m_alength + 0.001 ) ) ) {
         return -1.0;
      }

      if( m_style == Linear ) {
         return ( ( drawPos / m_alength ) * ( m_up - m_low ) + m_low );
      } else if( m_style == Logarithmic ) {
         // This should be calculated correctly later on:
         return -1.0;
      } else {
         return -1.0;
      }
   }

} // namespace moni
