// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_MONI_HISTOGRAM_H
#define CDA_GUI_MONI_HISTOGRAM_H

// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QWidget>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif // Q_OS_DARWIN

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QPainter )

/// Namespace for the monitoring classes
/**
 * All the classes responsible for online monitoring, which don't use
 * any CERNLIB or ROOT functionality, are put into this namespace.
 *
 * @author Attila Krasznahorkay Jr. <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace moni {

   // Bring the Qt objects into this namespace:
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QPainter );

   /**
    *  @short 1 dimensional histogram used in the DAQ monitoring
    *
    *         The "standard" way of monitoring a CDA DAQ session is to start the
    *         PAW global memory writing application, and monitor the histograms
    *         using PAW.
    *
    *         In the absence of CERNLIB however, one needs to provide the same
    *         kind of simple functionality in a Qt-only way. This histogram can
    *         be used to monitor a single variable read out from the CAMAC
    *         system.
    *
    * @author Attila Krasznahorkay Jr. <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Histogram : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-style constructor
      Histogram( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Constructor specifying all properties of the histogram
      Histogram( const QString& title, int bins, double low, double up,
                 QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Get the title of the histogram
      const QString& getTitle() const;
      /// Get the number of channels of the histogram
      int getBins() const;
      /// Get the lower bound of the hisrogram
      double getLowerBound() const;
      /// Get the upper bound of the historgam
      double getUpperBound() const;

      /// Possible styles for the axes
      enum AxisStyle {
         Linear = 0, ///< The axis should be drawn in a linear fashion
         Logarithmic = 1 ///< The axis should be drawn in a logarithmic fashion
      };

      /// Get the "style" of the X axis
      AxisStyle getXAxisStyle() const;
      /// Get the "style" of the Y axis
      AxisStyle getYAxisStyle() const;

   public slots:
      /// Set the title of the histogram
      void setTitle( const QString& title );
      /// Set the number of channels of the histogram
      void setBins( int bins );
      /// Set the lower bound of the hisrogram
      void setLowerBound( double low );
      /// Set the upper bound of the hisrogram
      void setUpperBound( double up );

      /// Set the "style" of the X axis
      void setXAxisStyle( AxisStyle style );
      /// Set the "style" of the Y axis
      void setYAxisStyle( AxisStyle style );

      /// Reset the contents of the histogram
      void reset();

      /// Fill the histogram
      void fill( double value, double weight = 1.0 );

   protected:
      /// Re-implemented function, used to draw the histogram
      virtual void paintEvent( QPaintEvent* event );

   private:
      /// Structure describing the visual binning of the axes
      struct AxisBinning {
         int n_major_ticks; ///< Number of "major" ticks on the axis
         int n_minor_ticks; ///< Number of "minor" ticks on the axis
         double tick_major_offset; ///< Offset of the first "major" tick
         double tick_minor_offset; ///< Offset of the first "minor" tick
         double tick_major_unit; ///< Distance of "major" ticks in axis units
         double tick_minor_unit; ///< Distance of "minor" ticks in axis units
         double tick_major_draw_unit; ///< Distance of "major" ticks in pixels
         double tick_minor_draw_unit; ///< Distance of "minor" ticks in pixels
         double first_major_tick_value; ///< Value of the first "major" tick
      }; // class AxisBinning

      /// Get the bin representing this value
      size_t getBin( double value ) const;
      /// Draw the X axis
      void drawXAxis( QPainter& painter );
      /// Draw the Y axis
      void drawYAxis( QPainter& painter );
      /// Draw the histogram itself
      void drawHist( QPainter& painter );
      /// Draw some statistics on top of the histogram
      void drawStat( QPainter& painter );
      /// Function determining the visual binning for a linear axis
      AxisBinning getLinearAxisBinning( double low, double up, double alength ) const;
      /// Function determining the visual binning for a logarithmic axis
      AxisBinning getLogarithmicAxisBinning( double low, double up, double alength ) const;
      /// Get the minimum and maximum for the Y axis
      std::pair< double, double > getYAxisLimits() const;

      /// Length of the ticks that have values associated to them
      static const quint32 TICK_LENGTH_MAJOR;
      /// Length of the minor ticks
      static const quint32 TICK_LENGTH_MINOR;
      /// Minimal distance between the major ticks
      static const quint32 MIN_TICK_DISTANCE;

      /// Space left under the X axis
      static const quint32 X_AXIS_SPACING;
      /// Space left on the side of the Y axis
      static const quint32 Y_AXIS_SPACING;

      QString m_title; ///< The title of the histogram
      int m_bins; ///< The number of channels
      double m_low; ///< The lower bound of the histogram
      double m_up; ///< The upper bound of the histogram

      AxisStyle m_xAxisStyle; ///< Style of the X axis
      AxisStyle m_yAxisStyle; ///< Style of the Y axis

      std::vector< double > m_values; ///< The values of the histogram
      int m_entries; ///< Number of times the fill function was called

      mutable msg::Logger m_logger; ///< The message logger object

   }; // class Histogram

} // namespace moni

#endif // CDA_GUI_MONI_HISTOGRAM_H
