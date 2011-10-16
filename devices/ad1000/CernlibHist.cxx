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
#include "CernlibHist.h"

namespace ad1000 {

   CernlibHist::CernlibHist()
      : m_histNumber( 0 ), m_logger( "ad1000::CernlibHist" ) {

   }

   bool CernlibHist::initialize( cernlib::HistMgr& hmgr ) {

      m_logger << msg::DEBUG << tr( "Initialising histogram(s)" ) << msg::endmsg;

      // Create a fancy name for the histogram:
      char name[ 80 ];
      sprintf( name, "%s (%s slot %i)",
               m_channel.getName().toLatin1().constData(),
               deviceName().toLatin1().constData(), getID() );

      // Book the histogram and remember its ID:
      m_histNumber = hmgr.book_1d( name,
                                   m_channel.getNumberOfChannels(),
                                   m_channel.getLowerBound(),
                                   m_channel.getUpperBound() );

      m_logger << msg::VERBOSE
               << tr( "Created histogram %1 with name: \"%2\"" )
         .arg( m_histNumber ).arg( name )
               << msg::endmsg;

      return true;
   }

   bool CernlibHist::displayEvent( const ev::Fragment& fragment,
                                   const cernlib::HistMgr& hmgr ) const {

      const std::vector< uint32_t >& dataWords = fragment.getDataWords();

      // Sanity check:
      if( dataWords.size() != 1 ) {
         m_logger << msg::ERROR << "Received " << dataWords.size() << " data words. "
                  << "Was supposed to receive 1." << msg::endmsg;
         return false;
      }

      // Decode the data word:
      unsigned int data = ( dataWords.front() & 0xffffff );

      // Fill the histogram:
      hmgr.fill_1d( m_histNumber, data, 1. );

      return true;
   }

} // namespace ad1000
