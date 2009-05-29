// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/camac/Crate.h"
#else
#   include "camac/Crate.h"
#endif

// Local include(s):
#include "Readout.h"

namespace t4300b {

   Readout::Readout()
      : m_logger( "t4300b::Readout" ) {

   }

   /**
    * The initialisation codes are hard-coded into this function.
    * It should be okay, as this is the only class that has to access
    * the device anyway.
    */
   bool Readout::initialize( camac::Crate& crate ) const {

      if( m_generateLam ) {
         m_logger << msg::DEBUG << "Initialising T4300B QDC in slot "
                  << m_slot << " to generate LAM" << msg::endmsg;
         crate.writeWord( m_slot, 0, 16, 0x4800 );
      } else {
         m_logger << msg::DEBUG << "Initialising T4300B QDC in slot "
                  << m_slot << " NOT to generate LAM" << msg::endmsg;
         crate.writeWord( m_slot, 0, 16, 0x800 );
      }

      return true;

   }

   /**
    * The function creates a new event fragment and configures it with
    * this device's module number. Then it reads out all the configured
    * subaddresses of the device and adds one data word for each of them
    * to the event fragment. In the data fragment both the subaddress of
    * the channel producing the data, and the data itself are encoded
    * the following way:
    *
    *   - uppermost 8 bits: Subaddress number
    *   - lower 24 bits: Data
    */
   ev::Fragment Readout::readEvent( camac::Crate& crate ) const {

      ev::Fragment fragment;
      fragment.setModuleNumber( m_slot );

      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            uint32_t channel = crate.readWord( m_slot, i, 2 );
            uint32_t dword = ( i << 24 ) | ( channel & 0xffffff );
            fragment.addDataWord( dword );
            m_logger << msg::VERBOSE << "From subaddress " << i
                     << " : " << channel << msg::endmsg;
         }
      }

      return fragment;

   }

   bool Readout::clear( camac::Crate& crate ) const {

      //
      // Clear the module:
      //
      crate.writeWord( m_slot, 0, 9, 0 );

      m_logger << msg::VERBOSE << "Cleared module" << msg::endmsg;

      return true;

   }

} // namespace t4300b
