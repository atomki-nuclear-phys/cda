// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#   include "cdacore/cernlib/HistMgr.h"
#else
#   include "event/Fragment.h"
#   include "cernlib/HistMgr.h"
#endif

// Local include(s):
#include "CernlibHist.h"

namespace ad413a {

   CernlibHist::CernlibHist()
      : m_logger( "ad413a::CernlibHist" ) {

      // Clear the histogram table:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_histTable[ i ] = 0;
      }
   }

   bool CernlibHist::initialize( cernlib::HistMgr& hmgr ) {

      m_logger << msg::DEBUG
               << tr( "Initialising histograms" )
               << msg::endmsg;

      // Loop over all configured subaddresses:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {

            // Book the histogram and register it in the ID table:
            m_histTable[ i ] =
               hmgr.book_1d( tr( "%1 (%2 slot %3 channel %4)" )
                             .arg( m_channels[ i ]->getName() )
                             .arg( deviceName() )
                             .arg( getID() ).arg( i ).toLatin1().constData(),
                             m_channels[ i ]->getNumberOfChannels(),
                             m_channels[ i ]->getLowerBound(),
                             m_channels[ i ]->getUpperBound() );

            REPORT_VERBOSE( tr( "Created histogram %1 with name: \"%2\"" )
                            .arg( m_histTable[ i ] )
                            .arg( m_channels[ i ]->getName() ) );
         }
      }

      return true;
   }

   bool CernlibHist::displayEvent( const ev::Fragment& fragment,
                                   const cernlib::HistMgr& hmgr ) const {

      // Access the data words:
      const ev::Fragment::Payload_t& dataWords = fragment.getDataWords();

      // Loop over all data words in the event fragment:
      ev::Fragment::Payload_t::const_iterator dword_itr = dataWords.begin();
      ev::Fragment::Payload_t::const_iterator dword_end = dataWords.end();
      for( ; dword_itr != dword_end; ++dword_itr ) {

         // Decode the data word:
         const int subaddress    = ( *dword_itr >> 24 ) & 0xff;
         const unsigned int data = ( *dword_itr & 0xffffff );

         // Check that the decoded information makes sense:
         if( ! ( ( subaddress >= 0 ) &&
                 ( subaddress < NUMBER_OF_SUBADDRESSES ) &&
                 m_channels[ subaddress ] ) ) {
            REPORT_ERROR( tr( "Received data word from unknown channel" ) );
            return false;
         }

         // Fill the histogram:
         hmgr.fill_1d( m_histTable[ subaddress ], ( double ) data, 1. );
      }

      return true;
   }

} // namespace ad413a
