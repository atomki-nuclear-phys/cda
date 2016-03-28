// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#include "common/errorcheck.h"
#include "root/NTupleMgr.h"
#include "event/Fragment.h"

// Local include(s):
#include "RootDisk.h"

namespace ad1000 {

   RootDisk::RootDisk()
      : m_logger( "ad1000::RootDisk" ) {

   }

   bool RootDisk::initialize( root::NTupleMgr& nmgr ) {

      // Add the single variable of the device:
      CHECK( nmgr.addVar( m_value, m_channel.getName() ) );

      return true;
   }

   bool RootDisk::writeEvent( const ev::Fragment& fragment ) const {

      // The data words in the event fragment:
      const ev::Fragment::Payload_t& dataWords = fragment.getDataWords();

      // Check that the fragment has only one data word:
      CHECK( dataWords.size() == 1 );

      // Decode the data word:
      m_value = ( dataWords.front() & 0xffffff );

      return true;
   }

} // namespace ad1000
