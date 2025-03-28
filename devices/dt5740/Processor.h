// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_PROCESSOR_H
#define CDA_DEVICES_DT5740_PROCESSOR_H

// System include(s):
#include <cstdint>
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "msg/Logger.h"

namespace dt5740 {

/**
 *  @short Class holding the signal reconstruction code
 *
 *         All the code used for reconstructing the signal shapes coming
 *         from the DT5740 is collected in this class.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class Processor {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(dt5740::Processor)

public:
   /**
    *  @short Result structure for the signal reconstruction
    *
    *         The reconstruction creates just a few simple propeties
    *         out of the signal shapes collected by the DT5740. This structure
    *         describes those reconstructed properties.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   struct Result {
      double time;    ///< Mean value of the reconstructed peak
      double energy;  ///< Total energy found in the signal
   };  // class Result

   /// Constructor
   Processor();

   /// Reconstruct the properties of the digitized signal
   virtual bool reconstruct(const std::vector<uint16_t>& data, Result& result);

   /// Get the "fraction" value used by the digital CFD
   double getFraction() const;
   /// Set the "fraction" value used by the digital CFD
   void setFraction(double value);

   /// Get the delay used by the digital CFD
   int getDelay() const;
   /// Set the delay used by the digital CFD
   void setDelay(int value);

   /// Get the length of the samples used by the digital CFD
   int getLength() const;
   /// Set the length of the samples used by the digital CFD
   void setLength(int value);

   /// Get the width of the signal smoothing area
   double getSmoothWidth() const;
   /// Set the width of the signal smoothing area
   void setSmoothWidth(double value);

protected:
   /// Function calculating the 1-normalized Gaussian distribution
   double gauss(int x) const;

   double m_fraction;  ///< "Fraction" used by the digital CFD
   int m_delay;        ///< Delay used by the digital CFD
   int m_length;       ///< Length used by the digital CFD

   double m_smoothWidth;  ///< Width of the signal smoothing area

   std::vector<double> m_smooth;  ///< Smoothed values
   std::vector<double> m_trans;   ///< CFD transformed values

private:
   mutable msg::Logger m_logger;  ///< Message logger object

};  // class Processor

}  // namespace dt5740

#endif  // CDA_DEVICES_DT5740_PROCESSOR_H
