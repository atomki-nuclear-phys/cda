// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/caen/Digitizer.h"
#else
#   include "caen/Digitizer.h"
#endif

// Local include(s):
#include "Crate.h"

namespace caen_reader {

   Crate::Crate()
      : dev::Crate< dev::CaenReadout >(),
        m_logger( "caen_reader::Crate" ) {

   }

   bool Crate::initialize( caen::Digitizer& dgtz ) const {

      // The crate should only contain one device:
      if( m_devices.size() != 1 ) {
         m_logger << msg::ERROR
                  << tr( "Crate doesn't contain exactly one device. "
                         "Initialization failed." )
                  << msg::endmsg;
         return false;
      }

      // Initialize this one device:
      return m_devices.begin()->second->initialize( dgtz );
   }

   /**
    * Even though in the current setup we will always only have
    * one device in this "crate", in order to be compatible with the
    * other parts of CDA, one has to package the data coming from this
    * one device into a full ev::Event object.
    *
    * @param dgtz The object to access the CAEN digitizer with
    * @returns A full even read from the device(s)
    */
   ev::Event Crate::readEvent( caen::Digitizer& dgtz ) const {

      // The event object to return:
      ev::Event event;

      // The only device in this "crate":
      const dev::CaenReadout* device = m_devices.begin()->second;

      // Wait for events to become available, if there's nothing
      // to read out right now.
      while( ! device->eventsAvailable( dgtz ) ) {
         // Wait for an IRQ for the next 100 ms:
         dgtz.irqWait( 100 );
      }

      // Read out the event from the single constituent:
      event.addFragment( m_devices.begin()->second->readEvent( dgtz ) );

      // Return the event:
      return event;
   }

} // namespace caen_reader
