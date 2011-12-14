// $Id$

// Local include(s):
#include "CernlibDisk.h"

namespace dt5740 {

   CernlibDisk::CernlibDisk()
      : dev::CernlibDisk(), Device(),
        m_logger( "dt5740::CernlibDisk" ) {

   }

   bool CernlibDisk::initialize( cernlib::NTupleMgr& /*nmgr*/ ) {

      return true;
   }

   bool CernlibDisk::writeEvent( const ev::Fragment& /*fragment*/,
                                 cernlib::NTupleMgr& /*nmgr*/ ) const {

      return true;
   }

} // namespace dt5740
