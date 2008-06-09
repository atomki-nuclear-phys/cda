
// Local include(s):
#include "Hist.h"

namespace camac {

   Hist::Hist()
      : dev::Hist(), Config(), m_logger( "Example::Hist" ) {

   }

   bool Hist::initialize() {

      m_logger << msg::INFO << "Initialized for writing histograms"
               << msg::endmsg;

      return true;

   }

   bool Hist::displayEvent( const ev::Fragment& /*fragment*/ ) {

      m_logger << msg::DEBUG << "Added one event fragment to histograms"
               << msg::endmsg;

      return true;

   }

} // namespace camac
