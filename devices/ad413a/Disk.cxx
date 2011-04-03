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

namespace ad413a {

   Disk::Disk()
      : m_logger( "ad413a::Disk" ) {

      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_ntupleTable[ i ] = 0;
      }

   }

   bool Disk::initialize( cernlib::NTupleMgr& nmgr ) {

      m_logger << msg::DEBUG << tr( "Initialising ntuple output" ) << msg::endmsg;

      // Loop over all configured subaddresses:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {

            m_ntupleTable[ i ] = nmgr.addVar( m_channels[ i ]->getName() );

         }
      }

      return true;
   }

   bool Disk::writeEvent( const ev::Fragment& fragment,
                          cernlib::NTupleMgr& nmgr ) const {

      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      for( std::vector< uint32_t >::const_iterator dword = dataWords.begin();
           dword != dataWords.end(); ++dword ) {

         // Decode the data word:
         int subaddress      = ( *dword >> 24 ) & 0xff;
         unsigned int chdata = ( *dword & 0xffffff );

         // Check that the decoded information makes sense:
         if( ( subaddress >= 0 ) && ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
             m_channels[ subaddress ] ) {

            // Fill the subaddress data in the ntuple manager:
            if( ! nmgr.setVar( m_ntupleTable[ subaddress ],
                               ( float ) chdata ) ) {
               m_logger << msg::ERROR << tr( "There was a problem filling one of "
                                             "the variables" )
                        << msg::endmsg;
               return false;
            }

         } else {
            m_logger << msg::ERROR << tr( "Received data word from unknown channel" )
                     << msg::endmsg;
            return false;
         }

      }

      return true;
   }

} // namespace ad413a
