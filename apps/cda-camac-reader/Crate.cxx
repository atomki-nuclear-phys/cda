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
      : dev::Crate< dev::CamacReadout >(),
        m_logger( "reader::Crate" ) {

      m_logger << msg::VERBOSE << tr( "Object constructed" ) << msg::endmsg;
   }

   /**
    * The destructor doesn't really have to do anything...
    */
   Crate::~Crate() {

      m_logger << msg::VERBOSE << tr( "Object destructed" ) << msg::endmsg;
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

      m_logger << msg::INFO << tr( "Initializing CAMAC devices" ) << msg::endmsg;

      crate.initialize();
      crate.clear();

      std::map< unsigned int, dev::CamacReadout* >::const_iterator dev_itr =
         m_devices.begin();
      std::map< unsigned int, dev::CamacReadout* >::const_iterator dev_end =
         m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {

         if( ! dev_itr->second->initialize( crate ) ) {
            m_logger << msg::ERROR
                     << tr( "There was a problem initializing one "
                            "of the devices" )
                     << msg::endmsg;
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
         m_logger << msg::ERROR
                  << tr( "There was a problem enabling interrupts\n"
                         "for the CAMAC crate" )
                  << msg::endmsg;
         return event;
      }

      //      crate.resetInhibit();

      //
      // Wait for LAM signal:
      //
      if( ! crate.waitForLAM() ) {
         m_logger << msg::ERROR
                  << tr( "There was a problem while waiting for LAM\n"
                         "Returning empty event..." )
                  << msg::endmsg;
         return event;
      } else {
         m_logger << msg::VERBOSE << tr( "Received LAM signal" )
                  << msg::endmsg;
      }

      //
      // Read out the event fragments from all the devices:
      //
      std::map< unsigned int, dev::CamacReadout* >::const_iterator dev_itr =
         m_devices.begin();
      std::map< unsigned int, dev::CamacReadout* >::const_iterator dev_end =
         m_devices.end();
      for( ; dev_itr != dev_end; ++dev_itr ) {

         event.addFragment( dev_itr->second->readEvent( crate ) );
      }

      return event;
   }

   bool Crate::clear( camac::Crate& crate ) const {

      crate.clear();

      /*
      for( std::map< unsigned int, dev::CamacReadout* >::const_iterator device =
              m_devices.begin(); device != m_devices.end(); ++device ) {

         if( ! device->second->clear( crate ) ) {
            m_logger << msg::ERROR << "There was a problem clearing one "
                     << "of the devices" << msg::endmsg;
            return false;
         }

      }

      crate.clearLAM();
      */

      m_logger << msg::VERBOSE << tr( "Cleared crate" ) << msg::endmsg;

      return true;
   }

} // namespace reader
