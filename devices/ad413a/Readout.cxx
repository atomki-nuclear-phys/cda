// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/camac/Crate.h"
#   include "cdacore/event/Fragment.h"
#else
#   include "camac/Crate.h"
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Readout.h"

namespace ad413a {

   Readout::Readout()
      : m_logger( "ad413a::Readout" ) {

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

      // Write control register 1:
      if( m_generateLam ) {
         m_logger << msg::DEBUG
                  << tr( "Initialising AD413A ADC in slot %1"
                         " to generate LAM" ).arg( m_slot )
                  << msg::endmsg;
         crate.writeWord( m_slot, 0, 16, 0xe300 );
      } else {
         m_logger << msg::DEBUG
                  << tr( "Initialising AD413A ADC in slot %1"
                         " NOT to generate LAM" ).arg( m_slot )
                  << msg::endmsg;
         crate.writeWord( m_slot, 0, 16, 0xa300 );
      }

      // Write control register 2:
      if( m_gate == 0 ) {
         crate.writeWord( m_slot, 1, 16, 0x0 );
      } else if( m_gate == 1 ) {
         crate.writeWord( m_slot, 1, 16, 0x1e );
      } else if( m_gate == 2 ) {
         crate.writeWord( m_slot, 1, 16, 0x1d );
      } else if( m_gate == 3 ) {
         crate.writeWord( m_slot, 1, 16, 0x1b );
      } else if( m_gate == 4 ) {
         crate.writeWord( m_slot, 1, 16, 0x17 );
      } else {
         m_logger << msg::WARNING
                  << tr( "Gate value is %1\n"
                         "Will enable all gates for now..." ).arg( m_gate )
                  << msg::endmsg;
         crate.writeWord( m_slot, 1, 16, 0x0 );
      }

      // Set the channel LLD values:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            crate.writeWord( m_slot, i, 17,
                             m_channels[ i ]->getLowLevelDiscriminant() );
         }
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
   UniquePtr< ev::Fragment >::Type
   Readout::readEvent( camac::Crate& crate ) const {

      // Create the event fragment:
      UniquePtr< ev::Fragment >::Type fragment( new ev::Fragment() );
      fragment->setModuleID( m_slot );

      // Read out all the configured channels:
      for( int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i ) {
         if( m_channels[ i ] ) {
            const uint32_t channel = crate.readWord( m_slot, i, 2 );
            const uint32_t dword = ( i << 24 ) | ( channel & 0xffffff );
            fragment->addDataWord( dword );
         }
      }

      return fragment;
   }

   bool Readout::clear( camac::Crate& crate ) const {

      //
      // Clear the module:
      //
      crate.writeWord( m_slot, 0, 10, 0 );

      return true;
   }

} // namespace ad413a
