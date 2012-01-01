// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/event/Fragment.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#else
#   include "common/errorcheck.h"
#   include "event/Fragment.h"
#   include "cernlib/NTupleMgr.h"
#endif

// Local include(s):
#include "CernlibDisk.h"

namespace ad1000 {

   CernlibDisk::CernlibDisk()
      : m_ntupleNumber( 0 ), m_logger( "ad1000::CernlibDisk" ) {

   }

   bool CernlibDisk::initialize( cernlib::NTupleMgr& nmgr ) {

      m_logger << msg::DEBUG
               << tr( "Initialising ntuple output" )
               << msg::endmsg;

      // Add the one variable:
      m_ntupleNumber = nmgr.addVar( m_channel.getName() );

      return true;
   }

   bool CernlibDisk::writeEvent( const ev::Fragment& fragment,
                                 cernlib::NTupleMgr& nmgr ) const {

      // Access the data words:
      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Sanity check:
      if( dataWords.size() != 1 ) {
         REPORT_ERROR( tr( "Received %1 data words. "
                           "Was supposed to receive 1." )
                       .arg( dataWords.size() ) );
         return false;
      }

      // Decode the data word:
      const uint32_t chdata = ( dataWords.front() & 0xffffff );

      // Fill the channel data in the ntuple manager:
      CHECK( nmgr.setVar( m_ntupleNumber, ( float ) chdata ) );

      return true;
   }

} // namespace ad1000
