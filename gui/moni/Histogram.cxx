// $Id$

// System include(s):
#include <algorithm>
#include <cmath>

// Qt include(s):
#include <QtCore/QPoint>
#include <QtCore/QLine>
#include <QtCore/QRect>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>
#include <QtGui/QActionGroup>

// Local include(s):
#include "Histogram.h"
#include "AxisStyleAction.h"

namespace moni {

   //
   // Set the values of all the constants:
   //
   const int Histogram::TICK_LENGTH_MAJOR = 15;
   const int Histogram::TICK_LENGTH_MINOR = 8;
   const int Histogram::MIN_TICK_DISTANCE = 80;

   const int Histogram::X_AXIS_SPACING = 80;
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
        m_low( 0.0 ), m_up( 100.0 ), m_xAxisStyle( Linear ), m_yAxisStyle( Linear ),
        m_values( 102, 0.0 ), m_entries( 0 ),
        m_logger( "moni::Histogram" ) {

      setMinimumSize( 220, 200 );
      setMaximumSize( 2200, 2000 );

      reset();
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
    * @param parent The Qt widget parent of the histogram
    * @param flags Qt specific flags for the object
    */
   Histogram::Histogram( const QString& title, int bins, double low, double up,
                         QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_title( title ), m_nbins( bins ),
        m_low( low ), m_up( up ), m_xAxisStyle( Linear ), m_yAxisStyle( Linear ),
        m_values( bins + 2, 0.0 ), m_entries( 0 ),
        m_logger( "moni::Histogram" ) {

      setMinimumSize( 220, 200 );
      setMaximumSize( 2200, 2000 );

      reset();
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
      reset();
      return;
   }

   void Histogram::setUpperBound( double up ) {

      m_up = up;
      reset();
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

   /**
    * This function clears the contents of the histogram, updating
    * its internal cache to be in sync with the current configuration.
    * It also performs a simple check on the X axis limits.
    */
   void Histogram::reset() {

      m_values.clear();
      m_values.resize( m_nbins + 2, 0.0 );
      m_entries = 0;

      // Check if the configured values make sense:
      if( ! ( m_up > m_low ) ) {
         m_logger << msg::ERROR << "The upper bound of histogram '" << m_title
                  << "' is not larger than its lower bound" << msg::endmsg;
      }

      update();
      return;
   }

   /**
    * Function used to fill the histogram. Just like in ROOT, it is
    * possible to fill entries using a weight, but in the monitoring this
    * is probably not going to be used.
    *
    * @param value The value at which the histogram should be filled
    * @param weight The weight with which the histogram should be filled
    */
   void Histogram::fill( double value, double weight ) {

      // Check if the given parameters make sense:
      if( std::isnan( value ) || std::isnan( weight ) ) {
         m_logger << msg::ERROR << "fill( value = " << value << ", weight = " << weight
                  << " ): NaN received" << msg::endmsg;
         return;
      }

      // Fill the histogram:
      m_values[ getBin( value ) ] += weight;
      ++m_entries;

      // Re-draw the widget:
      update();

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
      painter.drawRect( QRect( 0, 0, width(), height() ) );

      // Draw the rectangle of the histogram:
      painter.drawRect( QRect( Y_AXIS_SPACING, 20, width() - ( Y_AXIS_SPACING + 20 ),
                               height() - ( X_AXIS_SPACING + 20 ) ) );

      // Draw the different parts of the historgam:
      drawXAxis( painter );
      drawYAxis( painter );
      drawHist( painter );
      drawStat( painter );

      // Finally, show the title of the histogram:
      painter.setPen( Qt::SolidLine );
      painter.drawText( QRect( 0, height() - ( X_AXIS_SPACING - TICK_LENGTH_MAJOR -
                                               30 ), width(), 25 ),
                        Qt::AlignCenter, m_title );

      return;
   }

   void Histogram::mousePressEvent( QMouseEvent* event ) {

      // Return right away if it's not a right click:
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
      AxisStyleAction* xLogAction = new AxisStyleAction( Logarithmic,
                                                         tr( "Logarithmic" ),
                                                         xAxisMenu );
      connect( xLogAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
               this, SLOT( setXAxisStyle( Histogram::AxisStyle ) ) );

      // Group these actions together:
      QActionGroup* xAxisStyleGroup = new QActionGroup( xAxisMenu );
      xAxisStyleGroup->addAction( xLinAction );
      xAxisStyleGroup->addAction( xLogAction );
      if( getXAxisStyle() == Linear ) {
         xLinAction->setChecked( true );
      } else if( getXAxisStyle() == Logarithmic ) {
         xLogAction->setChecked( true );
      }

      // Add the two actions to the sub-menu:
      xAxisMenu->addAction( xLinAction );
      xAxisMenu->addAction( xLogAction );

      // Sub-menu for changing the Y axis style:
      QMenu* yAxisMenu = menu->addMenu( tr( "Set Y axis style" ) );
      AxisStyleAction* yLinAction = new AxisStyleAction( Linear, tr( "Linear" ),
                                                         yAxisMenu );
      connect( yLinAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
               this, SLOT( setYAxisStyle( Histogram::AxisStyle ) ) );
      AxisStyleAction* yLogAction = new AxisStyleAction( Logarithmic,
                                                         tr( "Logarithmic" ),
                                                         yAxisMenu );
      connect( yLogAction, SIGNAL( triggered( Histogram::AxisStyle ) ),
               this, SLOT( setYAxisStyle( Histogram::AxisStyle ) ) );

      // Group these actions together:
      QActionGroup* yAxisStyleGroup = new QActionGroup( yAxisMenu );
      yAxisStyleGroup->addAction( yLinAction );
      yAxisStyleGroup->addAction( yLogAction );
      if( getYAxisStyle() == Linear ) {
         yLinAction->setChecked( true );
      } else if( getYAxisStyle() == Logarithmic ) {
         yLogAction->setChecked( true );
      }

      // Add the two actions to the sub-menu:
      yAxisMenu->addAction( yLinAction );
      yAxisMenu->addAction( yLogAction );

      // Action clearing the histogram:
      QAction* resetAction = menu->addAction( tr( "Clear histogram" ) );
      connect( resetAction, SIGNAL( triggered() ),
               this, SLOT( reset() ) );

      // Display the menu under the cursor:
      menu->exec( mapToGlobal( QPoint( event->x(), event->y() ) ) );

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
      if( value > m_up ) return ( m_nbins + 1 );

      return static_cast< size_t >( std::floor( ( value - m_low ) /
                                                ( ( m_up - m_low ) / m_nbins ) ) + 1 );
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
         const int tick_location = axis_start + major_itr->position();
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
         painter.drawText( QRect( tick_location - MIN_TICK_DISTANCE / 2,
                                  height() - ( X_AXIS_SPACING - 20 ), MIN_TICK_DISTANCE,
                                  20 ),
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
         const int tick_location = axis_start + minor_itr->position();
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
         const int tick_location = axis_start - major_itr->position();
         if( ( tick_location < 20 ) || ( tick_location > axis_start ) ) continue;

         // Draw the line of the tick:
         painter.setPen( Qt::SolidLine );
         painter.drawLine( QLine( Y_AXIS_SPACING - TICK_LENGTH_MAJOR, tick_location,
                                  Y_AXIS_SPACING, tick_location ) );

         // Construct the proper tick value:
         double tick_value = major_itr->value();
         if( exp.first ) {
            tick_value /= std::pow( 10.0, exp.second );
         }
         // Now draw it on the axis:
         painter.drawText( QRect( 5, tick_location - MIN_TICK_DISTANCE / 2,
                                  Y_AXIS_SPACING - TICK_LENGTH_MAJOR - 3,
                                  MIN_TICK_DISTANCE ),
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
         const int tick_location = axis_start - minor_itr->position();
         if( ( tick_location < 20 ) || ( tick_location > axis_start ) ) continue;
         painter.drawLine( QLine( Y_AXIS_SPACING - TICK_LENGTH_MINOR, tick_location,
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
      const double bin_width = ( m_up - m_low ) / static_cast< double >( m_nbins );

      // Determine the binning for the X axis:
      const AxisBinning xbin = getXAxisBinning();
      // Determine the binning for the Y axis:
      const AxisBinning ybin = getYAxisBinning();

      // Draw the histogram:
      int prev_pos = -1;
      for( int i = 0; i < m_nbins; ++i ) {
         // Security check:
         if( ybin.getDrawPosition( m_values[ i + 1 ] ) < 0.0 ) {
            prev_pos = y_axis_start;
            continue;
         }

         const int y_bin_pos = y_axis_start - ybin.getDrawPosition( m_values[ i + 1 ] );
         const int x_bin_low_pos = xbin.getDrawPosition( m_low + i * bin_width );
         const int x_bin_up_pos  = xbin.getDrawPosition( m_low + ( i + 1 ) * bin_width );

         // Another security check:
         if( ( x_bin_low_pos < 0.0 ) || ( x_bin_up_pos < 0.0 ) ) continue;

         painter.setPen( QPen( QBrush( QColor( Qt::darkRed ) ), 2 ) );
         if( prev_pos > 0 ) {
            painter.drawLine( QLine( x_axis_start + x_bin_low_pos, prev_pos,
                                     x_axis_start + x_bin_low_pos, y_bin_pos ) );
         }
         prev_pos = y_bin_pos;
         painter.drawLine( QLine( x_axis_start + x_bin_low_pos, y_bin_pos,
                                  x_axis_start + x_bin_up_pos, y_bin_pos ) );
         painter.setBrush( QBrush( Qt::darkRed, Qt::DiagCrossPattern ) );
         painter.drawRect( QRect( x_axis_start + x_bin_low_pos, y_bin_pos,
                                  x_bin_up_pos - x_bin_low_pos,
                                  height() - y_bin_pos - X_AXIS_SPACING ) );
      }

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

      // Return right away if the widget is not large enough to hold the statistics
      // information:
      if( ( width() < 400 ) || ( height() < 300 ) ) {
         return;
      }

      // Draw the frame of the statistics info:
      painter.setPen( Qt::SolidLine );
      painter.setBrush( Qt::white );
      painter.drawRect( QRect( width() - 170, 20, 150, 80 ) );

      // Draw the statistics as simple text:
      painter.drawText( QRect( width() - 160, 25, 130, 70 ),
                        Qt::AlignLeft | Qt::AlignVCenter,
                        tr( "Entries: %1\n"
                            "Underflows: %2\n"
                            "Overflows: %3" ).arg( m_entries ).arg( m_values.front() )
                        .arg( m_values.back() ) );

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
    * @returns A structure describing the proposed axis decoration
    */
   Histogram::AxisBinning Histogram::getLinearAxisBinning( double low, double up,
                                                           double alength ) const {

      // The unit length in pixels:
      const double axis_unit = alength / ( up - low );

      //
      // The major tick distance in the axis units:
      //
      double tick_major_unit = std::pow( 10.0, std::ceil( std::log10( up - low ) ) -
                                         1.0 );
      while( tick_major_unit * axis_unit < MIN_TICK_DISTANCE ) {
         tick_major_unit *= 2.0;
      }
      while( ( tick_major_unit * axis_unit / 2 ) > MIN_TICK_DISTANCE ) {
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
      const int n_major_ticks = std::ceil( ( up - low ) / tick_major_unit ) + 1.0;
      const int n_minor_ticks = std::floor( ( up - low ) / tick_minor_unit ) + 1.0;

      //
      // Offsets of the first major and minor tick marks:
      //
      const double tick_major_offset = std::fmod( -1.0 * low, tick_major_unit ) *
         axis_unit;
      const double tick_minor_offset = std::fmod( -1.0 * low, tick_minor_unit ) *
         axis_unit;

      //
      // The value associated with the first major tick mark:
      //
      double first_major_tick_value = 0.0;
      if( std::abs( low ) < 0.001 ) {
         first_major_tick_value = 0.0;
      } else {
         if( low < 0.0 ) {
            first_major_tick_value = std::fmod( std::abs( low ), tick_major_unit ) + low;
         } else {
            first_major_tick_value = std::fmod( std::abs( low ), tick_major_unit ) - low;
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
    * logarithmic layout. The returned structure describes how the axis should be
    * drawn.
    *
    * @param low The lower edge on the axis
    * @param up  The upper edge on the axis
    * @param alength The length of the axis in pixels
    * @returns A structure describing the proposed axis decoration
    */
   Histogram::AxisBinning Histogram::getLogarithmicAxisBinning( double low, double up,
                                                                double alength ) const {

      // Check if this binning is possible:
      if( ! ( low > 0.0 ) ) {
         m_logger << msg::ERROR << "Unable to use logarithmic style with limits ["
                  << low << "," << up << "]" << msg::endmsg;
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
      double tick_major_exponent_unit =
         std::pow( 10.0, std::ceil( std::log10( up_log -
                                                low_log ) ) - 1.0 );
      while( tick_major_exponent_unit * axis_unit < MIN_TICK_DISTANCE ) {
         tick_major_exponent_unit *= 2.0;
      }
      while( ( tick_major_exponent_unit * axis_unit / 2 ) > MIN_TICK_DISTANCE ) {
         tick_major_exponent_unit /= 2.0;
      }

      //
      // Unit(s) for the drawing:
      //
      const double tick_major_draw_unit = tick_major_exponent_unit * axis_unit;

      //
      // Number of ticks for the axis:
      //
      const int n_major_ticks =
         std::ceil( ( up_log - low_log ) / tick_major_exponent_unit ) + 1.0;

      //
      // Offset of the first major tick mark:
      //
      const double tick_major_offset =
         std::fmod( std::abs( low_log ), tick_major_exponent_unit ) * axis_unit;

      //
      // The value associated with the first major tick mark:
      //
      const double first_major_tick_value =
         std::fmod( std::abs( low_log ), tick_major_exponent_unit ) + low_log;

      // Create the result structure:
      AxisBinning result( Logarithmic, low, up, alength );

      // ...and now fill it:
      for( int i = 0; i < n_major_ticks; ++i ) {
         const double position = tick_major_offset + i * tick_major_draw_unit;
         if( ( position < 0.0 ) || ( position > alength ) ) continue;
         result.addMajorTick( AxisBinning::MajorTick( tick_major_offset +
                                                      i * tick_major_draw_unit,
                                                      std::pow( 10.0,
                                                                first_major_tick_value +
                                                                i * tick_major_exponent_unit ) ) );
      }
      for( int i = 0; i < n_major_ticks + 1; ++i ) {
         const double previous =
            std::pow( 10.0, first_major_tick_value +
                      ( i - 1 ) * tick_major_exponent_unit );
         const double current  =
            std::pow( 10.0, first_major_tick_value + i * tick_major_exponent_unit );
         for( int j = 0; j < 9; ++j ) {
            const double value = previous + j * ( current - previous ) / 9.0;
            if( ( value < low ) || ( value > up ) ) continue;
            result.addMinorTick( AxisBinning::MinorTick( result.getDrawPosition( value ) ) );
         }
      }

      return result;
   }

   /**
    * This function is used internally to figure out the minimum and maximum values for the
    * Y axis. The return value's first element stores the minimum value on the Y axis, while
    * the second element stores the maximum value.
    *
    * @returns The limits for the Y axis
    */
   std::pair< double, double > Histogram::getYAxisLimits() const {

      const double maximum =
         *( std::max_element( m_values.begin(), m_values.end() ) );
      double view_maximum = 0.0;
      const double minimum =
         *( std::min_element( m_values.begin(), m_values.end() ) );
      double view_minimum = 0.0;
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
         m_logger << msg::ERROR << "Unknown Y axis style!" << msg::endmsg;
         view_maximum = maximum;
         view_minimum = minimum;
      }

      return std::make_pair( view_minimum, view_maximum );
   }

   Histogram::AxisBinning Histogram::getXAxisBinning() const {

      // Length of the X axis:
      const int axis_length = width() - ( Y_AXIS_SPACING + 20 );

      // Decide upon the correct axis binning:
      if( m_xAxisStyle == Linear ) {
         return getLinearAxisBinning( m_low, m_up, axis_length );
      } else if( m_xAxisStyle == Logarithmic ) {
         return getLogarithmicAxisBinning( m_low, m_up, axis_length );
      }

      // Handle programming errors gracefully:
      m_logger << msg::ERROR << "Binning style for the X axis (" << m_xAxisStyle
               << ") not understood, using linear binning" << msg::endmsg;
      return getLinearAxisBinning( m_low, m_up, axis_length );
   }

   Histogram::AxisBinning Histogram::getYAxisBinning() const {

      // Get the limits for the Y axis:
      const std::pair< double, double > limits = getYAxisLimits();
      // Length of the Y axis:
      const int axis_length = height() - ( X_AXIS_SPACING + 20 );

      // Decide upon the correct axis binning:
      if( m_yAxisStyle == Linear ) {
         return getLinearAxisBinning( limits.first, limits.second, axis_length );
      } else if( m_yAxisStyle == Logarithmic ) {
         return getLogarithmicAxisBinning( limits.first, limits.second, axis_length );
      }

      // Handle programming errors gracefully:
      m_logger << msg::ERROR << "Binning style for the Y axis (" << m_yAxisStyle
               << ") not understood, using linear binning" << msg::endmsg;
      return getLinearAxisBinning( limits.first, limits.second, axis_length );
   }

   std::pair< bool, int >
   Histogram::getExponent( const Histogram::AxisBinning& abin ) const {

      if( abin.getStyle() == Linear ) {
         if( ( abin.majors().front().value() < -1000.0 ) ||
             ( abin.majors().back().value() > 1000.0 ) ) {
            // Decide on the exponent based on the largest major tick:
            int exponent =
               static_cast< int >( std::floor( std::log10( std::max( std::abs( abin.majors().front().value() ),
                                                                     std::abs( abin.majors().back().value() ) ) ) ) );
            return std::make_pair( true, exponent );
         } else {
            return std::make_pair( false, -1 );
         }
      } else if( abin.getStyle() == Logarithmic ) {
         if( abin.majors().back().value() > 1000.0 ) {
            // Take an average of the exponents:
            int exponent = 0.0;
            std::vector< AxisBinning::MajorTick >::const_iterator itr =
               abin.majors().begin();
            std::vector< AxisBinning::MajorTick >::const_iterator end =
               abin.majors().end();
            for( ; itr != end; ++itr ) {
               exponent += std::floor( std::log10( itr->value() ) );
            }
            exponent /= abin.majors().size();
            return std::make_pair( true, exponent );
         } else {
            return std::make_pair( false, -1 );
         }
      } else {
         m_logger << msg::ERROR << "Unknown axis style encountered" << msg::endmsg;
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

      if( ( pos < m_low ) || ( pos > m_up ) ) {
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

} // namespace moni
