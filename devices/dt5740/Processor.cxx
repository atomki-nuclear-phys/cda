// $Id$

// Local include(s):
#include "Processor.h"

namespace dt5740 {

   Processor::Processor()
      : m_logger( "dt5740::Processor" ) {

   }

   /**
    * This is the main signal reconstruction function of the device's
    * data processing.
    *
    * @param data The raw samples collected from a channel
    * @param result The reconstructed properties of the signal
    * @returns <code>true</code> if the singal reconstruction was successful,
    *          <code>false</code> otherwise
    */
   bool Processor::reconstruct( const std::vector< uint16_t >& /*data*/,
                                Result& result ) const {

      result.time   = 100.0;
      result.energy = 500.0;

      return true;
   }

} // namespace dt5740
