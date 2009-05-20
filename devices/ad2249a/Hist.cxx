// $Id$

// System include(s):
#include <cstdio>

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#else
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Hist.h"

namespace ad2249a {

   Hist::Hist()
      : m_logger( "ad2249a::Hist" ) {

      // Clear the histogram table:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_histTable[ i ] = 0;
      }

   }

   bool Hist::initialize( cernlib::HistMgr& hmgr ) {

      m_logger << msg::DEBUG << "Initialising histograms" << msg::endmsg;

      // Loop over all configured subaddresses:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {

            // Create a fancy name for the histogram:
            char name[ 80 ];
            sprintf( name, "%s (%s slot %i channel %i)",
                     m_channels[ i ]->getName().toLatin1().constData(),
                     type().toLatin1().constData(), getSlot(), i );

            // Book the histogram and register it in the ID table:
            m_histTable[ i ] = hmgr.book_1d( name,
                                             m_channels[ i ]->getNumberOfChannels(),
                                             m_channels[ i ]->getLowerBound(),
                                             m_channels[ i ]->getUpperBound() );

            m_logger << msg::VERBOSE
                     << tr( "Created histogram %1 with name: \"%2\"" )
               .arg( m_histTable[ i ] ).arg( name )
                     << msg::endmsg;

         }
      }

      return true;

   }

   bool Hist::displayEvent( const ev::Fragment& fragment,
                            const cernlib::HistMgr& hmgr ) const {

      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      for( std::vector< uint32_t >::const_iterator dword = dataWords.begin();
           dword != dataWords.end(); ++dword ) {

         // Decode the data word:
         int subaddress    = ( *dword >> 24 ) & 0xff;
         unsigned int data = ( *dword & 0xffffff );

         // Check that the decoded information makes sense:
         if( ( subaddress >= 0 ) && ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
             m_channels[ subaddress ] ) {

            // Fill the histogram:
            hmgr.fill_1d( m_histTable[ subaddress ], data, 1. );

         } else {
            m_logger << msg::ERROR << "Received data word from unknown channel"
                     << msg::endmsg;
            return false;
         }

      }

      return true;

   }

} // namespace ad2249a
