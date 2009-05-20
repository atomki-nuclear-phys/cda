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

namespace ad2249a {

   Readout::Readout()
      : m_logger( "ad2249a::Readout" ) {

   }

   /**
    * The initialisation codes are hard-coded into this function.
    * It should be okay, as this is the only class that has to access
    * the device anyway.
    */
   bool Readout::initialize( camac::Crate& crate ) const {

      //
      // Clear the module:
      //
      crate.writeWord( m_slot, 0, 9, 0 );

      if( m_generateLam ) {
         m_logger << msg::DEBUG << "Initialising AD2249A ADC in slot "
                  << m_slot << " to generate LAM" << msg::endmsg;
         crate.writeWord( m_slot, 0, 26, 0 );
      } else {
         m_logger << msg::DEBUG << "Initialising AD2249A ADC in slot "
                  << m_slot << " NOT to generate LAM" << msg::endmsg;
         crate.writeWord( m_slot, 0, 24, 0 );
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
    *
    * Since there are only 12 subaddresses on the device and it's an
    * 10-bit ADC (as far as I remember), this should leave plenty of space
    * for both quantities...
    */
   ev::Fragment Readout::readEvent( camac::Crate& crate ) const {
 	
      ev::Fragment fragment; 
      fragment.setModuleNumber( m_slot );

      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            uint32_t channel = crate.readWord( m_slot, i, 0 );
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
      crate.writeWord( m_slot, 0, 10, 0 );

      m_logger << msg::VERBOSE << "Cleared module" << msg::endmsg;

      return true;

   }

} // namespace ad2249a
