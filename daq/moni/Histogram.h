// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_MONI_HISTOGRAM_H
#define CDA_DAQ_MONI_HISTOGRAM_H

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QTimer>
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
                 int refreshTimeout = 2000,
                 QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Get the title of the histogram
      const QString& getTitle() const;
      /// Get the number of channels of the histogram
      int getNBins() const;
      /// Get the lower bound of the hisrogram
      double getLowerBound() const;
      /// Get the upper bound of the historgam
      double getUpperBound() const;

      /// Get the refresh timeout in miliseconds
      int getRefreshTimeout() const;
      /// Set the refresh timeout in miliseconds
      void setRefreshTimeout( int value );

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
      void setNBins( int bins );
      /// Set the lower bound of the hisrogram
      void setLowerBound( double low );
      /// Set the upper bound of the hisrogram
      void setUpperBound( double up );

      /// Set the color of the histogram
      void setHistColor( const QColor& color );

      /// Set the "style" of the X axis
      void setXAxisStyle( Histogram::AxisStyle style );
      /// Set the "style" of the Y axis
      void setYAxisStyle( Histogram::AxisStyle style );

      /// Reset the contents of the histogram
      void reset();

      /// Fill the histogram
      void fill( double value, double weight = 1.0 );

   protected:
      /// Re-implemented function, used to draw the histogram
      virtual void paintEvent( QPaintEvent* event );
      /// Re-implemented function, used to draw the pop-up menu
      virtual void mousePressEvent( QMouseEvent* event );

      /**
       *  @short Internal class used in displaying the axis binning
       *
       *         Figuring out how to properly display an axis is not that trivial
       *         as it turns out. This class is used in the Histogram class internally
       *         to store how the binning should be done for a particular axis.
       *
       * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
       *
       * $Revision$
       * $Date$
       */
      class AxisBinning {

      public:
         /// Type of the tick positions (in pixels)
         typedef double tick_position;
         /// Typedef for the tick values
         typedef double tick_value;

         /// The properties of one "major" tick
         class MajorTick {
         public:
            /// Constructor with a position and an associated value
            MajorTick( tick_position position, tick_value value );
            /// Get the position of this tick
            tick_position position() const;
            /// Get the value associated with this tick
            tick_value value() const;
         private:
            tick_position m_position; ///< Tick position
            tick_value m_value; ///< Tick value
         }; // class MajorTick

         /// The properties of one "minor" tick
         class MinorTick {
         public:
            /// Constructor with a position
            MinorTick( tick_position position );
            /// Get the position of this tick
            tick_position position() const;
         private:
            tick_position m_position; ///< Tick position
         }; // class MinorTick

         /// Default constructor
         AxisBinning( AxisStyle style = Linear, double low = 0.0, double up = 100.0,
                      double alength = 100.0 );

         /// Add one "major" tick to the axis view
         void addMajorTick( const MajorTick& tick );
         /// Add one "minor" tick to the axis view
         void addMinorTick( const MinorTick& tick );

         /// Get all the "major" tick information
         const std::vector< MajorTick >& majors() const;
         /// Get all the "minor" tick information
         const std::vector< MinorTick >& minors() const;

         /// Get the "style" of the axis
         AxisStyle getStyle() const;

         /// Get the position of where to draw a given value
         double getDrawPosition( double pos ) const;

      private:
         std::vector< MajorTick > m_majors; ///< Description of the "major" ticks
         std::vector< MinorTick > m_minors; ///< Description of the "minor" ticks
         const AxisStyle m_style; ///< "Style" of the axis
         const double m_low; ///< Value belonging to the lower edge of the axis
         const double m_up; ///< Value belonging to the upper edge of the axis
         const double m_alength; ///< Length of the virtual axis
      }; // class AxisBinning

      /// Get the bin representing this value
      virtual size_t getBin( double value ) const;
      /// Draw the X axis
      virtual void drawXAxis( QPainter& painter ) const;
      /// Draw the Y axis
      virtual void drawYAxis( QPainter& painter ) const;
      /// Draw the histogram itself
      virtual void drawHist( QPainter& painter ) const;
      /// Draw some statistics on top of the histogram
      virtual void drawStat( QPainter& painter ) const;
      /// Function determining the visual binning for a linear axis
      AxisBinning getLinearAxisBinning( double low, double up,
                                        double alength, int tdist ) const;
      /// Function determining the visual binning for a logarithmic axis
      AxisBinning getLogarithmicAxisBinning( double low, double up,
                                             double alength, int tdist ) const;
      /// Get the minimum and maximum for the Y axis
      virtual std::pair< double, double > getYAxisLimits() const;
      /// Get whether logarithmic axis binning is possible for the X axis
      virtual bool getLogXPossible() const;
      /// Get whether logarithmic axis binning is possible for the Y axis
      virtual bool getLogYPossible() const;
      /// Get the binning for the X axis
      virtual AxisBinning getXAxisBinning() const;
      /// Get the binning for the Y axis
      virtual AxisBinning getYAxisBinning() const;
      /// Get a possible exponent that should be used for displaying an axis
      std::pair< bool, int > getExponent( const AxisBinning& abin ) const;

      /// Length of the ticks that have values associated to them
      static const int TICK_LENGTH_MAJOR;
      /// Length of the minor ticks
      static const int TICK_LENGTH_MINOR;
      /// Minimal distance between the major ticks on the X axis
      static const int MIN_X_TICK_DISTANCE;
      /// Minimal distance between the major ticks on the Y axis
      static const int MIN_Y_TICK_DISTANCE;

      /// Space left under the X axis
      static const int X_AXIS_SPACING;
      /// Space left on the side of the Y axis
      static const int Y_AXIS_SPACING;

      QString m_title; ///< The title of the histogram
      int m_nbins; ///< The number of channels
      double m_low; ///< The lower bound of the histogram
      double m_up; ///< The upper bound of the histogram

      QColor m_hColor; ///< Color for the histogram

      QTimer m_refreshTimer; ///< Timer for refreshing the histogram

      AxisStyle m_xAxisStyle; ///< Style of the X axis
      AxisStyle m_yAxisStyle; ///< Style of the Y axis

      std::vector< double > m_values; ///< The values of the histogram
      int m_entries; ///< Number of times the fill function was called

      mutable msg::Logger m_logger; ///< The message logger object

   }; // class Histogram

} // namespace moni

#endif // CDA_DAQ_MONI_HISTOGRAM_H
