// $Id$

// System include(s):
#include <algorithm>
#include <cmath>

// Qt include(s):
#include <QtCore/QPoint>
#include <QtCore/QLine>
#include <QtCore/QRect>
#include <QtGui/QPainter>

// Local include(s):
#include "Histogram.h"

namespace moni {

   //
   // Set the values of all the constants:
   //
   const quint32 Histogram::TICK_LENGTH_MAJOR = 15;
   const quint32 Histogram::TICK_LENGTH_MINOR = 8;
   const quint32 Histogram::MIN_TICK_DISTANCE = 80;

   const quint32 Histogram::X_AXIS_SPACING = 80;
   const quint32 Histogram::Y_AXIS_SPACING = 80;

   /**
    * Constructor mostly useful when the properties of the histogram are
    * not yet known at construction. The histogram specific members are
    * filled in with some defaults when using this constructor.
    *
    * @param parent The Qt widget parent of the histogram
    * @param flags Qt specific flags for the object
    */
   Histogram::Histogram( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_title( "N/A" ), m_bins( 100 ),
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
      : QWidget( parent, flags ), m_title( title ), m_bins( bins ),
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

   int Histogram::getBins() const {

      return m_bins;
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
      return;
   }

   void Histogram::setBins( int bins ) {

      m_bins = bins;
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
      m_values.resize( m_bins + 2, 0.0 );
      m_entries = 0;

      // Check if the configured values make sense:
      if( ! ( m_up > m_low ) ) {
         m_logger << msg::ERROR << "The upper bound of histogram '" << m_title
                  << "' is not larger than its lower bound" << msg::endmsg;
      }

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

   /**
    * A very simple function for determining which bin represents a given
    * value.
    *
    * @param value A value on the X axis
    * @returns The bin representing the specified value
    */
   size_t Histogram::getBin( double value ) const {

      if( value < m_low ) return 0;
      if( value > m_up ) return ( m_bins + 1 );

      return static_cast< size_t >( std::floor( ( value - m_low ) /
                                                ( ( m_up - m_low ) / m_bins ) ) + 1 );
   }

   /**
    * This function draws the X axis of the histogram. It just nicely labels the
    * axis depending on the settings of the histogram.
    *
    * @param painter The painter object to use for the drawing
    */
   void Histogram::drawXAxis( QPainter& painter ) {

      //
      // Some basic values about the axis:
      //
      const int axis_length = width() - ( Y_AXIS_SPACING + 20 );
      const int axis_start = Y_AXIS_SPACING;

      // Determine the binning for the axis:
      const AxisBinning abin = getLinearAxisBinning( m_low, m_up, axis_length );

      // Decide whether to print the axis values with exponents:
      bool writeExponent = false;
      int exponent = -1;
      if( ( m_low < -1000.0 ) || ( m_up > 1000.0 ) ) {
         writeExponent = true;
         exponent = static_cast< int >( std::floor( std::log10( std::max( std::abs( m_up ),
                                                                          std::abs( m_low ) ) ) ) );
      }

      // Draw the "major ticks" together with the labels:
      for( int i = 0; i < abin.n_major_ticks; ++i ) {

         // Calculate the position of the tick:
         const int tick_location = axis_start + abin.tick_major_offset + i * abin.tick_major_draw_unit;
         if( tick_location > ( width() - 20 ) ) break;

         // Draw the line of the tick:
         painter.setPen( Qt::SolidLine );
         painter.drawLine( QLine( tick_location, height() - X_AXIS_SPACING,
                                  tick_location, height() - X_AXIS_SPACING + TICK_LENGTH_MAJOR ) );

         // Construct the proper tick value:
         double tick_value = abin.first_major_tick_value + i * abin.tick_major_unit;
         if( writeExponent ) {
            tick_value /= std::pow( 10.0, exponent );
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
      if( writeExponent ) {
         painter.drawText( QRect( width() - 70, height() - X_AXIS_SPACING + 35,
                                  50, 20 ),
                           Qt::AlignCenter,
                           // This should be painted much nicer once I'll have time:
                           QString( "x10^%1" ).arg( exponent ) );
      }

      // Draw the "minor ticks":
      painter.setPen( Qt::SolidLine );
      for( int i = 0; i < abin.n_minor_ticks; ++i ) {
         const int tick_location = axis_start + abin.tick_minor_offset + i * abin.tick_minor_draw_unit;
         if( tick_location > ( width() - 20 ) ) break;
         painter.drawLine( QLine( tick_location, height() - X_AXIS_SPACING,
                                  tick_location, height() - X_AXIS_SPACING + TICK_LENGTH_MINOR ) );
      }

      return;
   }

   /**
    * This function draws the Y axis of the histogram. It just nicely labels the
    * axis depending on the contents of the histogram.
    *
    * @param painter The painter object to use for the drawing
    */
   void Histogram::drawYAxis( QPainter& painter ) {

      // Get the limits for the Y axis:
      std::pair< double, double > limits = getYAxisLimits();

      //
      // Some basic values about the axis:
      //
      const int axis_length = height() - ( X_AXIS_SPACING + 20 );
      const int axis_start = height() - X_AXIS_SPACING;

      // Determine the binning for the axis:
      const AxisBinning abin = getLinearAxisBinning( limits.first, limits.second, axis_length );

      // Decide whether to print the axis values with exponents:
      bool writeExponent = false;
      int exponent = -1;
      if( ( limits.first < -1000.0 ) || ( limits.second > 1000.0 ) ) {
         writeExponent = true;
         exponent = static_cast< int >( std::floor( std::log10( std::max( std::abs( limits.first ),
                                                                          std::abs( limits.second ) ) ) ) );
      }

      // Draw the "major ticks" together with the labels:
      for( int i = 0; i < abin.n_major_ticks; ++i ) {

         // Calculate the position of the tick:
         const int tick_location = axis_start - abin.tick_major_offset - i * abin.tick_major_draw_unit;
         if( tick_location < 20 ) break;

         // Draw the line of the tick:
         painter.setPen( Qt::SolidLine );
         painter.drawLine( QLine( Y_AXIS_SPACING - TICK_LENGTH_MAJOR, tick_location,
                                  Y_AXIS_SPACING, tick_location ) );

         // Construct the proper tick value:
         double tick_value = abin.first_major_tick_value + i * abin.tick_major_unit;
         if( writeExponent ) {
            tick_value /= std::pow( 10.0, exponent );
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
      if( writeExponent ) {
         painter.drawText( QRect( 5, 5, 50, 20 ),
                           Qt::AlignCenter,
                           // This should be painted much nicer once I'll have time:
                           QString( "x10^%1" ).arg( exponent ) );
      }

      // Draw the "minor ticks":
      painter.setPen( Qt::SolidLine );
      for( int i = 0; i < abin.n_minor_ticks; ++i ) {
         const int tick_location = axis_start - abin.tick_minor_offset - i * abin.tick_minor_draw_unit;
         if( tick_location < 20 ) break;
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
   void Histogram::drawHist( QPainter& painter ) {

      //
      // Some basic values about the drawing area:
      //
      const int x_axis_length = width() - ( Y_AXIS_SPACING + 20 );
      const int x_axis_start = Y_AXIS_SPACING;
      const double bin_width = x_axis_length / static_cast< double >( m_bins );
      const int y_axis_length = height() - ( X_AXIS_SPACING + 20 );
      const int y_axis_start = height() - X_AXIS_SPACING;

      // Get the limits for the Y axis:
      std::pair< double, double > limits = getYAxisLimits();

      // Draw the histogram:
      int prev_pos = -1;
      for( int i = 0; i < m_bins; ++i ) {
         const int y_bin_pos = y_axis_start - ( m_values[ i + 1 ] - limits.first ) /
            ( limits.second - limits.first ) * y_axis_length;
         painter.setPen( QPen( QBrush( QColor( Qt::darkRed ) ), 2 ) );
         if( prev_pos > 0 ) {
            painter.drawLine( QLine( x_axis_start + i * bin_width, prev_pos,
                                     x_axis_start + i * bin_width, y_bin_pos ) );
         }
         prev_pos = y_bin_pos;
         painter.drawLine( QLine( x_axis_start + i * bin_width, y_bin_pos,
                                  x_axis_start + ( i + 1 ) * bin_width, y_bin_pos ) );
         painter.setBrush( QBrush( Qt::darkRed, Qt::DiagCrossPattern ) );
         painter.drawRect( QRect( x_axis_start + i * bin_width, y_bin_pos,
                                  bin_width, height() - y_bin_pos - X_AXIS_SPACING ) );
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
   void Histogram::drawStat( QPainter& painter ) {

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

      // Create the result structure:
      AxisBinning result;

      // The unit length in pixels:
      const double axis_unit = alength / ( up - low );

      //
      // The major tick distance in the axis units:
      //
      result.tick_major_unit = std::pow( 10.0, std::ceil( std::log10( up - low ) ) - 1.0 );
      while( result.tick_major_unit * axis_unit < MIN_TICK_DISTANCE ) {
         result.tick_major_unit *= 2.0;
      }
      while( ( result.tick_major_unit * axis_unit / 2 ) > MIN_TICK_DISTANCE ) {
         result.tick_major_unit /= 2.0;
      }

      //
      // The minor tick distance in axis units:
      //
      result.tick_minor_unit = result.tick_major_unit / 10.0;

      //
      // Units for the drawing:
      //
      result.tick_major_draw_unit = result.tick_major_unit * axis_unit;
      result.tick_minor_draw_unit = result.tick_minor_unit * axis_unit;

      //
      // Number of ticks for the axis:
      //
      result.n_major_ticks = std::ceil( ( up - low ) / result.tick_major_unit ) + 1.0;
      result.n_minor_ticks = std::floor( ( up - low ) / result.tick_minor_unit ) + 1.0;

      //
      // Offsets of the first major and minor tick marks:
      //
      result.tick_major_offset = std::fmod( std::abs( low ), result.tick_major_unit ) * axis_unit;
      result.tick_minor_offset = std::fmod( std::abs( low ), result.tick_minor_unit ) * axis_unit;

      //
      // The value associated with the first major tick mark:
      //
      result.first_major_tick_value = std::fmod( std::abs( low ), result.tick_major_unit ) + low;

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

      // Create the result structure:
      AxisBinning result;

      // Function will have to be implemented...

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
      const double view_maximum = ( std::abs( maximum ) > 0.001 ? maximum * 1.1 : 10.0 );

      const double minimum =
         *( std::min_element( m_values.begin(), m_values.end() ) );
      const double view_minimum = ( std::abs( minimum ) > 0.001 ? minimum * 1.1 : 0.0 );

      return std::make_pair( view_minimum, view_maximum );
   }

} // namespace moni
