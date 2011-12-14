// $Id$

// Local include(s):
#include "CernlibHist.h"

namespace dt5740 {

   CernlibHist::CernlibHist()
      : dev::CernlibHist(), Device(),
        m_logger( "dt5740::CernlibHist" ) {

   }

   bool CernlibHist::initialize( cernlib::HistMgr& /*hmgr*/ ) {

      return true;
   }

   bool CernlibHist::displayEvent( const ev::Fragment& /*fragment*/,
                                   const cernlib::HistMgr& /*hmgr*/ ) const {

      return true;
   }

} // namespace dt5740
