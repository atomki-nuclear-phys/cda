// $Id$

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "Readout.h"

namespace v862 {

   Readout::Readout()
      : Device(), m_logger( "v862::Readout" ) {

   }

   bool Readout::initialize( const caen::VmeBus& bus ) {

      // Open the connection to the hardware device:
      CHECK( m_vmeDevice.open( m_vmeAddress, caen::VmeDevice::DEV_V862,
                               bus ) );

      // Print some information about it. Making sure in the process that we
      // can talk to the device:
      CHECK( m_vmeDevice.printInfo( msg::DEBUG ) );

      // Clear all data from the device:
      CHECK( m_vmeDevice.dataClear() );

      // Return gracefully:
      return true;
   }

   bool Readout::finalize() {

      // Close the connection to the device:
      CHECK( m_vmeDevice.close() );

      // Return gracefully:
      return true;
   }

   bool Readout::start() {

      // Since the device can't really be turned off, just clear all current
      // data from the device, expecting that readout will start momentarily.
      CHECK( m_vmeDevice.dataClear() );

      // Return gracefully:
      return true;
   }

   bool Readout::stop() {

      // There's no easy way of preventing the device from running...

      // Return gracefully:
      return true;
   }

   std::unique_ptr< ev::Fragment > Readout::readEvent() {

      // Create the event fragment:
      std::unique_ptr< ev::Fragment > fragment( new ev::Fragment() );

      // If the event buffer is empty, fill it:
      if( ! m_events.size() ) {
         if( ! m_vmeDevice.read( m_events ) ) {
            REPORT_ERROR( tr( "Failed to read data from the device" ) );
            return fragment;
         }
         if( ! m_events.size() ) {
            REPORT_ERROR( tr( "Code logic error detected") );
            return fragment;
         }
      }

      // Translate the first event in the buffer into an event fragment:
      fragment->setModuleID( m_vmeAddress );
      for( const auto& data : m_events.front().data ) {
         // Construct a 32-bit word with all the data from the channel:
         const uint32_t dword = ( ( data.data & 0xffff ) |
                                  ( ( data.channel << 16 ) & 0xff0000 ) |
                                  ( data.underThreshold ? 0x1000000 : 0 ) |
                                  ( data.overflow ? 0x2000000 : 0 ) );
         fragment->addDataWord( dword );
      }

      // Return the event fragment:
      return fragment;
   }

} // namespace v862
