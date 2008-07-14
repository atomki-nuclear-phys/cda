// $Id$

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#   include "cdacore/camac/Crate.h"
#else
#   include "device/Factory.h"
#   include "camac/Crate.h"
#endif

// Local include(s):
#include "Crate.h"

namespace reader {

   /**
    * The constructor has to create the base class in a fancy way. It
    * specifies that the base class should use dev::Readout kind of
    * devices, and that it should use the dev::Factory::createReadout
    * function to create these devices when reading a configuration.
    */
   Crate::Crate()
      : dev::Crate< dev::Readout >( &dev::Factory::createReadout ),
        m_logger( "reader::Crate" ) {

      m_logger << msg::VERBOSE << "Object constructed" << msg::endmsg;

   }

   /**
    * The destructor doesn't really have to do anything...
    */
   Crate::~Crate() {

      m_logger << msg::VERBOSE << "Object destructed" << msg::endmsg;

   }

   /**
    * The function simply loops over all the CAMAC devices, and calls
    * dev::Readout::initialize() for each of them. This should be enough
    * to configure each of the devices for data acquisition.
    *
    * @param crate A correctly initialised connection to the CAMAC crate
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::initialize( camac::Crate& crate ) const {

      m_logger << msg::INFO << "Initializing CAMAC devices" << msg::endmsg;

      crate.initialize();
      crate.clear();

      for( std::map< int, dev::Readout* >::const_iterator device =
              m_devices.begin(); device != m_devices.end(); ++device ) {

         if( ! device->second->initialize( crate ) ) {
            m_logger << msg::ERROR << "There was a problem initializing one "
                     << "of the devices" << msg::endmsg;
            return false;
         }

      }

      // Enable all CAMAC interrupts:
      //      crate.writeLong( 28, 1, 16, 0x00ffffff );
      crate.setLAMMask( 0xffffff );

      return true;

   }

   /**
    * The function waits for a LAM signal to arrive, then reads out all
    * the devices which were configured. It packages the output from each
    * device into a single ev::Event object and returns this to the caller.
    *
    * @param crate A correctly initialised connection to the CAMAC crate
    * @returns The event that's just been read out
    */
   ev::Event Crate::readEvent( camac::Crate& crate ) const {

      ev::Event event;

      if( ! crate.enableInterrupt() ) {
         m_logger << msg::ERROR << "There was a problem enabling interrupts"
                  << std::endl
                  << "for the CAMAC crate." << msg::endmsg;
         return event;
      }

      //      crate.resetInhibit();

      //
      // Wait for LAM signal:
      //
      if( ! crate.waitForLAM() ) {
         m_logger << msg::ERROR << "There was a problem while waiting for LAM"
                  << std::endl
                  << "Returning empty event..." << msg::endmsg;
         return event;
      } else {
         m_logger << msg::VERBOSE << "Received LAM signal" << msg::endmsg;
      }

      //
      // Read out the event fragments from all the devices:
      //
      for( std::map< int, dev::Readout* >::const_iterator device =
              m_devices.begin(); device != m_devices.end(); ++device ) {

         event.addFragment( device->second->readEvent( crate ) );

      }

      return event;

   }

   bool Crate::clear( camac::Crate& crate ) const {

      crate.clear();

      for( std::map< int, dev::Readout* >::const_iterator device =
              m_devices.begin(); device != m_devices.end(); ++device ) {

         if( ! device->second->clear( crate ) ) {
            m_logger << msg::ERROR << "There was a problem clearing one "
                     << "of the devices" << msg::endmsg;
            return false;
         }

      }

      crate.clearLAM();

      m_logger << msg::VERBOSE << "Cleared crate" << msg::endmsg;

      return true;

   }

} // namespace reader
