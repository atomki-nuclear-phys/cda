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

namespace t4300b {

   CernlibDisk::CernlibDisk()
      : m_logger( "t4300b::CernlibDisk" ) {

      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_ntupleTable[ i ] = 0;
      }
   }

   bool CernlibDisk::initialize( cernlib::NTupleMgr& nmgr ) {

      m_logger << msg::DEBUG
               << tr( "Initialising ntuple output" )
               << msg::endmsg;

      // Loop over all configured subaddresses:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            // Book the variable:
            m_ntupleTable[ i ] = nmgr.addVar( m_channels[ i ]->getName() );
         }
      }

      return true;
   }

   bool CernlibDisk::writeEvent( const ev::Fragment& fragment,
                                 cernlib::NTupleMgr& nmgr ) const {

      // Access the data words:
      const ev::Fragment::Payload_t& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      ev::Fragment::Payload_t::const_iterator dword_itr = dataWords.begin();
      ev::Fragment::Payload_t::const_iterator dword_end = dataWords.end();
      for( ; dword_itr != dword_end; ++dword_itr ) {

         // Decode the data word:
         const int subaddress      = ( *dword_itr >> 24 ) & 0xff;
         const unsigned int chdata = ( *dword_itr & 0xffffff );

         // Check that the decoded information makes sense:
         if( ! ( ( subaddress >= 0 ) &&
                 ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
                 m_channels[ subaddress ] ) ) {
            REPORT_ERROR( tr( "Received data word from unknown channel" ) );
            return false;
         }

         // Fill the subaddress data in the ntuple manager:
         CHECK( nmgr.setVar( m_ntupleTable[ subaddress ],
                             ( float ) chdata ) );
      }

      return true;
   }

} // namespace t4300b
