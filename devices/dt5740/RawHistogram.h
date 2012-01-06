// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_RAWHISTOGRAM_H
#define CDA_DEVICES_DT5740_RAWHISTOGRAM_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdadaq/moni/Histogram.h"
#else
#   include "moni/Histogram.h"
#endif

// Local include(s):
#include "Processor.h"

namespace dt5740 {

   /**
    *  @short Specialized histogram for the raw DT5740 data
    *
    *         This small extension on top of the moni::Histogram class
    *         makes it possible to nicely display the reconstructed time
    *         and energy values on top of the raw signal shape.
    *
    *         It is also responsible for reconstructing the time and
    *         energy values in the Qt monitoring application.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class RawHistogram : public moni::Histogram,
                        public Processor {

      Q_OBJECT

   public:
      /// Standard QWidget-style constructor
      RawHistogram( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Constructor specifying all properties of the histogram
      RawHistogram( const QString& title, int bins, double low, double up,
                    int refreshTimeout = 2000,
                    QWidget* parent = 0, Qt::WindowFlags flags = 0 );

      /// Reconstruct the properties of the digitized signal
      virtual bool reconstruct( const std::vector< uint16_t >& data,
                                Processor::Result& result );

   protected:
      /// Re-implemented function, used to draw the histogram
      virtual void paintEvent( QPaintEvent* event );

      /// Draw some statistics on top of the histogram
      virtual void drawStat( QPainter& painter ) const;
      /// Get the minimum and maximum for the Y axis
      virtual std::pair< double, double > getYAxisLimits() const;
      /// Get whether logarithmic axis binning is possible for the Y axis
      virtual bool getLogYPossible() const;

   private:
      double m_time; ///< Reconstructed time
      double m_energy; ///< Reconstructed energy

      mutable double m_transScale; ///< Scaling for the transformed distribution

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class RawHistogram

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_RAWHISTOGRAM_H
