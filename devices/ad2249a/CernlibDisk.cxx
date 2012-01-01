// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#   include "cdacore/event/Fragment.h"
#else
#   include "common/errorcheck.h"
#   include "cernlib/NTupleMgr.h"
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "CernlibDisk.h"

namespace ad2249a {

   CernlibDisk::CernlibDisk()
      : m_logger( "ad2249a::CernlibDisk" ) {

      // Reset the "ntuple map":
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
            m_ntupleTable[ i ] = nmgr.addVar( m_channels[ i ]->getName() );
         }
      }

      return true;
   }

   bool CernlibDisk::writeEvent( const ev::Fragment& fragment,
                                 cernlib::NTupleMgr& nmgr ) const {

      // Access the data words:
      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      for( std::vector< uint32_t >::const_iterator dword = dataWords.begin();
           dword != dataWords.end(); ++dword ) {

         // Decode the data word:
         const int subaddress      = ( *dword >> 24 ) & 0xff;
         const unsigned int chdata = ( *dword & 0xffffff );

         // Check that the decoded information makes sense:
         if( ! ( ( subaddress >= 0 ) && ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
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

} // namespace ad2248a
