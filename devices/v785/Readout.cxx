// $Id$

// CDA include(s):
#include "common/errorcheck.h"
#include "caen/StopAcquisition.h"

// Local include(s):
#include "Readout.h"

namespace v785 {

   Readout::Readout()
      : Device(), m_logger( "v785::Readout" ) {

   }

   bool Readout::initialize( const caen::VmeBus& bus ) {

      // Open the connection to the hardware device:
      CHECK( m_vmeDevice.open( m_vmeAddress, caen::VmeDevice::DEV_V785A,
                               bus ) );

      // Print some information about it. Making sure in the process that we
      // can talk to the device:
      CHECK( m_vmeDevice.printInfo( msg::DEBUG ) );

      // Clear all data from the device:
      CHECK( m_vmeDevice.dataClear() );

      // Set up the zero suppression for the device:
      std::vector< uint16_t > thresholds( NUMBER_OF_CHANNELS, 0xff );
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            thresholds[ i ] = m_channels[ i ]->getThreshold();
         }
      }
      CHECK( m_vmeDevice.setZeroSuppression( m_zeroSuppressionEnabled, false,
                                             thresholds ) );

      // Set up the acquisition mode:
      CHECK( m_vmeDevice.setAcquisitionMode( false, m_zeroSuppressionEnabled,
                                             m_overflowSuppressionEnabled,
                                             m_validSuppressionEnabled, false,
                                             true, true ) );

      // Set up the readout mode:
      CHECK( m_vmeDevice.setReadoutMode( true, true, true ) );

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

      // Reset the event cache:
      m_events.clear();
      m_eventsProcessed = 0;

      // Return gracefully:
      return true;
   }

   bool Readout::stop() {

      // There's no easy way of preventing the device from running...

      // Return gracefully:
      return true;
   }

   size_t Readout::eventsProcessed() const {

      return m_eventsProcessed;
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
         // If the acquisition is to be stopped, return an empty
         // fragment:
         if( g_stopAcquisition ) {
            REPORT_VERBOSE( tr( "The acquisition is to be stopped. Returning "
                                "empty event fragment." ) );
            return fragment;
         }
         // If the acquisition was not supposed to stop, then we should have
         // some events available now...
         if( ! m_events.size() ) {
            REPORT_ERROR( tr( "Code logic error detected") );
            return fragment;
         }
      }

      // Translate the first event in the buffer into an event fragment:
      fragment->setModuleID( m_vmeAddress );
      for( const auto& data : m_events.front().data ) {
         // Skip channels that we didn't set up for reading. To save some
         // bandwidth. (If there's a big enough signal on the channel, it will
         // be read out, even if we didn't ask for it.)
         if( ! m_channels[ data.channel ] ) {
            continue;
         }
         // Construct a 32-bit word with all the data from the channel:
         ChannelData d( data );
         fragment->addDataWord( d );
      }

      // Update the number of events processed variable:
      m_eventsProcessed = m_events.front().footer.eventCount;

      // Remove this event from the cache:
      m_events.erase( m_events.begin() );

      // Return the event fragment:
      return fragment;
   }

} // namespace v785
