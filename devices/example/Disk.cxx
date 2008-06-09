
// Local include(s):
#include "Disk.h"

namespace camac {

   Disk::Disk()
      : dev::Disk(), Config(), m_logger( "Example::Disk" ) {

   }

   bool Disk::initialize() {

      m_logger << msg::INFO << "Initialized for writing output file(s)"
               << msg::endmsg;

      return true;

   }

   bool Disk::writeEvent( const ev::Fragment& /*fragment*/ ) {

      m_logger << msg::DEBUG << "Added one event fragment to the output"
               << msg::endmsg;

      return true;

   }

} // namespace camac
