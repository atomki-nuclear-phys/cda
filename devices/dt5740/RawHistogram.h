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

namespace dt5740 {

   /**
    *  @short Specialized histogram for the raw DT5740 data
    *
    *         This small extension on top of the moni::Histogram class
    *         makes it possible to nicely display the reconstructed time
    *         and energy values on top of the raw signal shape.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class RawHistogram : public moni::Histogram {

   public:
      /// Standard QWidget-style constructor
      RawHistogram( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Constructor specifying all properties of the histogram
      RawHistogram( const QString& title, int bins, double low, double up,
                    int refreshTimeout = 2000,
                    QWidget* parent = 0, Qt::WindowFlags flags = 0 );

   public slots:
      /// Set the reconstructed time parameter
      void setReconstructedTime( double value );
      /// Set the reconstructed energy parameter
      void setReconstructedEnergy( double value );

   protected:
      /// Re-implemented function, used to draw the histogram
      virtual void paintEvent( QPaintEvent* event );

   private:
      double m_time; ///< Reconstructed time
      double m_energy; ///< Reconstructed energy

   }; // class RawHistogram

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_RAWHISTOGRAM_H
