// $Id$

// CDA include(s):
#include "common/errorcheck.h"
#include "caen/StopAcquisition.h"

// Local include(s):
#include "Readout.h"

namespace v775 {

   /// "Offline" bit in the Bit Set 2 register
   static const uint16_t BIT_SET_2_OFFLINE_BIT = 0x2;

   Readout::Readout()
      : Device(), m_logger( "v775::Readout" ) {

   }

   StatusCode Readout::initialize( const caen::VmeBus& bus ) {

      // Open the connection to the hardware device:
      CHECK( m_vmeDevice.open( m_vmeAddress, caen::VmeDevice::DEV_V775A,
                               bus ) );

      // Print some information about it. Making sure in the process that we
      // can talk to the device:
      CHECK( m_vmeDevice.printInfo( msg::DEBUG ) );

      // Take the device offline:
      CHECK( m_vmeDevice.writeBitSet2Register( BIT_SET_2_OFFLINE_BIT ) );

      // Clear all data from the device:
      CHECK( m_vmeDevice.dataClear() );

      // Set up the zero suppression for the device:
      std::vector< uint16_t > thresholds( NUMBER_OF_CHANNELS, 0xff );
      for( size_t i = 0; i < NUMBER_OF_CHANNELS; ++i ) {
         if( m_channels[ i ] ) {
            thresholds[ i ] = m_channels[ i ]->getThreshold();
         }
      }
      static const bool STEP_THRESHOLD = false;
      CHECK( m_vmeDevice.setZeroSuppression( m_zeroSuppressionEnabled,
                                             STEP_THRESHOLD,
                                             thresholds ) );

      // Set up the acquisition mode:
      static const bool SLIDING_SCALE_ENABLE = false;
      static const bool EMPTY_ENABLE = true;
      static const bool COUNT_ALL_EVENTS = true;
      CHECK( m_vmeDevice.setAcquisitionMode( SLIDING_SCALE_ENABLE,
                                             m_zeroSuppressionEnabled,
                                             m_overflowSuppressionEnabled,
                                             m_validSuppressionEnabled,
                                             m_commonStopEnabled,
                                             EMPTY_ENABLE,
                                             COUNT_ALL_EVENTS ) );

      // Set the FSR register by abusing the pedestal setter function:
      CHECK( m_vmeDevice.setPedestal( m_fullScaleRangeValue ) );

      // Set up the readout mode:
      static const bool BUS_ERROR_ENABLE = true;
      static const bool BLOCK_END_ENABLE = true;
      static const bool ALIGN_64_ENABLE = true;
      CHECK( m_vmeDevice.setReadoutMode( BUS_ERROR_ENABLE,
                                         BLOCK_END_ENABLE,
                                         ALIGN_64_ENABLE ) );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Readout::finalize() {

      // Close the connection to the device:
      CHECK( m_vmeDevice.close() );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Readout::start() {

      // Clear all possible data from the device:
      CHECK( m_vmeDevice.dataClear() );

      // Reset the event cache:
      m_events.clear();
      m_eventsProcessed = 0;

      // Turn the data acquisition on:
      CHECK( m_vmeDevice.writeBitClear2Register( BIT_SET_2_OFFLINE_BIT ) );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   StatusCode Readout::stop() {

      // Turn the data acuisition off:
      CHECK( m_vmeDevice.writeBitSet2Register( BIT_SET_2_OFFLINE_BIT ) );

      // Return gracefully:
      return StatusCode::SUCCESS;
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

} // namespace v775
