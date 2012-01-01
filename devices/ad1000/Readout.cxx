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

namespace ad1000 {

   Readout::Readout()
      : m_logger( "ad1000::Readout" ) {

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
         m_logger << msg::DEBUG
                  << tr( "Initialising AD1000 ADC in slot %1 "
                         "to generate LAM" ).arg( m_slot )
                  << msg::endmsg;
         crate.writeWord( m_slot, 2, 24, 0 );
      } else {
         m_logger << msg::DEBUG
                  << tr( "Initialising AD1000 ADC in slot %1 "
                         "NOT to generate LAM" ).arg( m_slot )
                  << msg::endmsg;
         crate.writeWord( m_slot, 0, 24, 0 );
      }

      return true;
   }

   /**
    * The function creates a new event fragment and configures it with
    * this device's module number. Then it reads out all the one channel
    * of the device, and adds it as a single data word (without much
    * encoding) to the event fragment.
    */
   ev::Fragment* Readout::readEvent( camac::Crate& crate ) const {

      // Create the new event fragment:
      ev::Fragment* fragment = new ev::Fragment();
      fragment->setModuleID( m_slot );

      // Read the channel data:
      crate.writeWord( m_slot, 0, 16, 0 ); // I think this command initiates the data conversion...
      const uint32_t channel = crate.readWord( m_slot, 0, 0 );
      const uint32_t dword = channel & 0xffffff;
      fragment->addDataWord( dword );

      return fragment;
   }

   bool Readout::clear( camac::Crate& crate ) const {

      //
      // Clear the module:
      //
      if( m_generateLam ) {
         crate.writeWord( m_slot, 2, 24, 0 );
      } else {
         crate.writeWord( m_slot, 0, 24, 0 );
      }

      REPORT_VERBOSE( tr( "Cleared module" ) );

      return true;
   }

} // namespace ad1000
