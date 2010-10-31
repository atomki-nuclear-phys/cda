// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#else
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Disk.h"

namespace ad1000 {

   Disk::Disk()
      : m_ntupleNumber( 0 ), m_logger( "ad1000::Disk" ) {

   }

   bool Disk::initialize( cernlib::NTupleMgr& nmgr ) {

      m_logger << msg::DEBUG << "Initialising ntuple output" << msg::endmsg;

      // Add the one variable:
      m_ntupleNumber = nmgr.addVar( m_channel.getName() );

      return true;
   }

   bool Disk::writeEvent( const ev::Fragment& fragment,
                          cernlib::NTupleMgr& nmgr ) const {

      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Sanity check:
      if( dataWords.size() != 1 ) {
         m_logger << msg::ERROR << "Received " << dataWords.size() << " data words. "
                  << "Was supposed to receive 1." << msg::endmsg;
         return false;
      }

      // Decode the data word:
      unsigned int chdata = ( dataWords.front() & 0xffffff );

      // Fill the channel data in the ntuple manager:
      if( ! nmgr.setVar( m_ntupleNumber,
                         ( float ) chdata ) ) {
         m_logger << msg::ERROR << "There was a problem filling "
                  << "the variable(s)" << msg::endmsg;
         return false;
      }

      return true;
   }

} // namespace ad1000
