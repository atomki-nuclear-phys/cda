
// Local include(s):
#include "Readout.h"

namespace camac {

   Readout::Readout()
      : dev::Readout(), Config(), m_logger( "Example::Readout" ) {

   }

   bool Readout::initialize() const {

      m_logger << msg::INFO << "Readout device initialised"
               << msg::endmsg;

      return true;

   }

   ev::Fragment Readout::readEvent() const {

      m_logger << msg::DEBUG << "Empty message fragment created"
               << msg::endmsg;

      return ev::Fragment();

   }

} // namespace camac
