// $Id$

// System include(s):
#include <cstdio>

// CERNLIB include(s):
extern "C" {
#   include <cfortran/cfortran.h>
#   include <cfortran/hbook.h>
}

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

namespace t2228a {

   Hist::Hist()
      : m_logger( "t2228a::Hist" ) {

      // Clear the histogram table:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         m_histTable[ i ] = 0;
      }

   }

   bool Hist::initialize( unsigned int& counter ) {

      m_logger << msg::DEBUG << "Initialising histograms" << msg::endmsg;

      // Loop over all configured subaddresses:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {

            // Increment the counter:
            ++counter;

            // Create a fancy name for the histogram:
            char name[ 50 ];
            sprintf( name, "%s (%s slot %i channel %i)",
                     m_channels[ i ]->getName().toLatin1().constData(),
                     type().toLatin1().constData(), getSlot(), i );

            m_logger << msg::VERBOSE << "Creating histogram "
                     << counter << " with name: \""
                     << name << "\"" << msg::endmsg;

            // Book the histogram itself:
            HBOOK1( counter, name, m_channels[ i ]->getNumberOfChannels(),
                    m_channels[ i ]->getLowerBound(),
                    m_channels[ i ]->getUpperBound(), 0. );

            // Add an entry to the histogram table:
            m_histTable[ i ] = counter;

         }
      }

      return true;

   }

   bool Hist::displayEvent( const ev::Fragment& fragment ) {

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
            HF1( m_histTable[ subaddress ], data, 1. );

         } else {
            m_logger << msg::ERROR << "Received data word from unknown channel"
                     << msg::endmsg;
            return false;
         }

      }

      return true;

   }

} // namespace t2228a
