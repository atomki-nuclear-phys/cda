// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_WS_DARWIN
#   include "cdacore/common/errorcheck.h"
#   include "cdacore/root/NTupleMgr.h"
#   include "cdacore/event/Fragment.h"
#else
#   include "common/errorcheck.h"
#   include "root/NTupleMgr.h"
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "RootDisk.h"

namespace ad413a {

   RootDisk::RootDisk()
      : m_logger( "ad413a::RootDisk" ) {

   }

   bool RootDisk::initialize( root::NTupleMgr& nmgr ) {

      // Loop over all configured subaddresses:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            CHECK( nmgr.addVar( m_values[ i ], m_channels[ i ]->getName() ) );
         }
      }

      return true;
   }

   bool RootDisk::writeEvent( const ev::Fragment& fragment ) const {

      // Reset all the ntuple variables:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_values[ i ] = 0;
      }

      // The data words in the event fragment:
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

         // Set the variable:
         m_values[ subaddress ] = chdata;
      }

      return true;
   }

} // namespace ad413a
