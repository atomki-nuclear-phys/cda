// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_PROCESSOR_H
#define CDA_DEVICES_DT5740_PROCESSOR_H

// System include(s):
extern "C" {
#   include <stdint.h>
}

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_WS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif // Q_WS_DARWIN

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
      Q_DECLARE_TR_FUNCTIONS( dt5740::Processor )

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
         double time;   ///< Mean value of the reconstructed peak
         double energy; ///< Total energy found in the signal
      }; // class Result

      /// Constructor
      Processor();

      /// Reconstruct the properties of the digitized signal
      bool reconstruct( const std::vector< uint16_t >& data,
                        Result& result ) const;

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Processor

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_PROCESSOR_H
