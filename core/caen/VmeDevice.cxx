
// System include(s):
#include <cstring>
#include <cstdlib>

// CAEN include(s):
#ifdef HAVE_CAEN_QTP_LIBS
extern "C" {
#   include <cvt_board_commons.h>
#   include <cvt_common_defs.h>
#   include <cvt_V792.h>
}
#endif

// CDA include(s):
#include "msg/Logger.h"
#include "common/errorcheck.h"
#include "common/Sleep.h"

// Local include(s):
#include "caen/VmeDevice.h"
#include "caen/VmeBus.h"
#include "caen/StopAcquisition.h"

/// Private namespace for @c core/caen/VmeDevice.cxx.
namespace {

#ifdef HAVE_CAEN_QTP_LIBS
   /// Helper function converting between device types
   CVT_QTP_TYPES convert( caen::VmeDevice::DeviceType type ) {
      switch( type ) {
      case caen::VmeDevice::DEV_V775A:
         return CVT_V775_TYPE_A;
         break;
      case caen::VmeDevice::DEV_V785A:
         return CVT_V785_TYPE_A;
         break;
      case caen::VmeDevice::DEV_V792A:
         return CVT_V792_TYPE_A;
         break;
      case caen::VmeDevice::DEV_V862:
         return CVT_V862_TYPE_A;
         break;
      default:
         msg::Logger m_logger( "convert" );
         REPORT_ERROR( "Unknown device type received: " << type );
         return static_cast< CVT_QTP_TYPES >( 0 );
      }
   }

   /// Helper function decoding the GEO address of a data word
   uint32_t decodeGeo( uint32_t data, caen::VmeDevice::DeviceType type ) {
      switch( type ) {
      case caen::VmeDevice::DEV_V775A:
         return CVT_V775_GET_DATUM_GEO( data );
         break;
      case caen::VmeDevice::DEV_V785A:
         return CVT_V785_GET_DATUM_GEO( data );
         break;
      case caen::VmeDevice::DEV_V792A:
         return CVT_V792_GET_DATUM_GEO( data );
         break;
      case caen::VmeDevice::DEV_V862:
         return CVT_V862_GET_DATUM_GEO( data );
         break;
      default:
         msg::Logger m_logger( "decodeGeo" );
         REPORT_ERROR( "Unknown device type received: " << type );
         return 0;
      }
   }

   /// Helper function decoding the channel number of a data word
   uint32_t decodeChannel( uint32_t data, caen::VmeDevice::DeviceType type ) {
      switch( type ) {
      case caen::VmeDevice::DEV_V775A:
         return CVT_V775A_GET_DATUM_CH( data );
         break;
      case caen::VmeDevice::DEV_V785A:
         return CVT_V785A_GET_DATUM_CH( data );
         break;
      case caen::VmeDevice::DEV_V792A:
         return CVT_V792A_GET_DATUM_CH( data );
         break;
      case caen::VmeDevice::DEV_V862:
         return CVT_V862_GET_DATUM_CH( data );
         break;
      default:
         msg::Logger m_logger( "decodeChannel" );
         REPORT_ERROR( "Unknown device type received: " << type );
         return 0;
      }
   }

   /// Helper function decoding the data payload of a data word
   uint32_t decodeData( uint32_t data, caen::VmeDevice::DeviceType type ) {
      switch( type ) {
      case caen::VmeDevice::DEV_V775A:
         return CVT_V775_GET_DATUM_ADC( data );
         break;
      case caen::VmeDevice::DEV_V785A:
         return CVT_V785_GET_DATUM_ADC( data );
         break;
      case caen::VmeDevice::DEV_V792A:
         return CVT_V792_GET_DATUM_ADC( data );
         break;
      case caen::VmeDevice::DEV_V862:
         return CVT_V862_GET_DATUM_ADC( data );
         break;
      default:
         msg::Logger m_logger( "decodeData" );
         REPORT_ERROR( "Unknown device type received: " << type );
         return 0;
      }
   }

   /// Helper function decoding the under threshold bit of a data word
   uint32_t decodeUnderThreshold( uint32_t data,
                                  caen::VmeDevice::DeviceType type ) {
      switch( type ) {
      case caen::VmeDevice::DEV_V775A:
         return CVT_V775_GET_DATUM_UN( data );
         break;
      case caen::VmeDevice::DEV_V785A:
         return CVT_V785_GET_DATUM_UN( data );
         break;
      case caen::VmeDevice::DEV_V792A:
         return CVT_V792_GET_DATUM_UN( data );
         break;
      case caen::VmeDevice::DEV_V862:
         return CVT_V862_GET_DATUM_UN( data );
         break;
      default:
         msg::Logger m_logger( "decodeUnderThreshold" );
         REPORT_ERROR( "Unknown device type received: " << type );
         return 0;
      }
   }

   /// Helper function decoding the overflow bit of a data word
   uint32_t decodeOverflow( uint32_t data, caen::VmeDevice::DeviceType type ) {
      switch( type ) {
      case caen::VmeDevice::DEV_V775A:
         return CVT_V775_GET_DATUM_OV( data );
         break;
      case caen::VmeDevice::DEV_V785A:
         return CVT_V785_GET_DATUM_OV( data );
         break;
      case caen::VmeDevice::DEV_V792A:
         return CVT_V792_GET_DATUM_OV( data );
         break;
      case caen::VmeDevice::DEV_V862:
         return CVT_V862_GET_DATUM_OV( data );
         break;
      default:
         msg::Logger m_logger( "decodeOverflow" );
         REPORT_ERROR( "Unknown device type received: " << type );
         return 0;
      }
   }
#endif // HAVE_CAEN_QTP_LIBS

   /// Helper function for printing pretty messages
   const char* toString( caen::VmeDevice::DeviceType type ) {
      switch( type ){
         case caen::VmeDevice::DEV_V775A:
            return "V775A";
            break;
         case caen::VmeDevice::DEV_V785A:
            return "V785A";
            break;
         case caen::VmeDevice::DEV_V792A:
            return "V792A";
            break;
         case caen::VmeDevice::DEV_V862:
            return "V862";
            break;
         default:
            return "<unknown>";
      }
   }

   /// Helper function for printing vectors
   template< typename T >
   msg::Logger& operator<< ( msg::Logger& out, const std::vector< T >& vec ) {
      out << "[";
      for( size_t i = 0; i < vec.size(); ++i ) {
         out << vec[ i ];
         if( i < vec.size() - 1 ) {
            out << ", ";
         }
      }
      out << "]";
      return out;
   }

} // private namespace

namespace caen {

   /// Helper class holding on to data used by the QTP library
   ///
   /// The library specific data is put into this object instead of the main
   /// VmeDevice one, so that the clients wouldn't have to be exposed to the
   /// implementation details of the class.
   ///
   class VmeDevicePrivateData {

   public:
      /// Default constructor
      VmeDevicePrivateData() : m_nEvents( 0 ) {
#ifdef HAVE_CAEN_QTP_LIBS
         // Make sure that the data object is zeroed out at the start
         memset( &m_data, 0, sizeof( m_data ) );
#endif // HAVE_CAEN_QTP_LIBS
         m_buffer = new uint8_t[ BUFFER_SIZE ];
      }
      /// Destructor
      ~VmeDevicePrivateData() {
         delete[] m_buffer;
      }

      /// Size of the allocated data buffer
      static const size_t BUFFER_SIZE = 1024 * 1024;

      /// Accessor for the readout buffer
      uint8_t* buffer() { return m_buffer; }
      /// Accessor for the buffer usage variable
      uint32_t& bufferUsage() { return m_bufferUsage; }

      /// The number of events "simulated" by the object so far
      int& nEvents() { return m_nEvents; }

#ifdef HAVE_CAEN_QTP_LIBS
      /// Accessor for the data object
      cvt_V792_data* data() { return &m_data; }

   private:
      /// Data object used by the QTP library
      cvt_V792_data m_data;
#endif // HAVE_CAEN_QTP_LIBS

      /// Data buffer used by the QTP library
      uint8_t* m_buffer;
      /// Size of the buffer currently being used
      uint32_t m_bufferUsage;

      /// Number of events "simulated" by the object so far
      int m_nEvents;

   }; // class VmeDevicePrivateData

   VmeDevice::VmeDevice()
      : m_data( 0 ), m_type( DEV_UNKNOWN ), m_logger( "caen::VmeDevice" ) {

   }

   VmeDevice::~VmeDevice() {

      // Make sure that the device is properly disconnected from:
      if( isConnected() ) {
         close();
      }
   }

   /// This function can be used to open the connection to a CAEN VME device.
   ///
   /// @param address The address of the device inside the VME crate
   /// @param type The device type
   /// @param bus A VmeBus object that is already initialised, and connected to
   ///            the VME bus controller
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::open( uint16_t address, DeviceType type,
                               const VmeBus& bus ) {

      // Make sure that the data is cleared:
      resetData();

      // Set the device type:
      m_type = type;

      // Open the connection to the device:
#ifdef HAVE_CAEN_QTP_LIBS
      CHECK( cvt_V792_open( m_data->data(), address, bus.handle(),
                            ::convert( type ) ) );
#endif // HAVE_CAEN_QTP_LIBS
      REPORT_VERBOSE( tr( "Device \"%1\" opened on address %2" )
                      .arg( toString( type ) ).arg( address, 4, 16 ) );

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to close the connection to the CAEN VME
   /// device.
   ///
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::close() {

      // Make sure that we have a data object:
      if( ! m_data ) {
         m_logger << msg::WARNING << tr( "Trying to close a closed device" )
                  << msg::endmsg;
         return StatusCode::FAILURE;
      }

      // Close the connection to the device:
#ifdef HAVE_CAEN_QTP_LIBS
      CHECK( cvt_V792_close( m_data->data() ) );
#endif // HAVE_CAEN_QTP_LIBS
      REPORT_VERBOSE( tr( "Device type \"%1\" closed" )
                      .arg( toString( m_type ) ) );

      // Delete the data object from memory:
      delete m_data;
      m_data = 0;

      // Set the device type to unknown:
      m_type = DEV_UNKNOWN;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to print some user friendly information about
   /// the device that the connection was made with.
   ///
   /// @param level The output level at which the information should be printed
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::printInfo( msg::Level level ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Get the information about the board:
      uint16_t firmware_rev = 0, serial_number = 0;
      uint8_t piggy_back_type = 0;
      CHECK( cvt_V792_get_system_info( m_data->data(), &firmware_rev,
                                       &piggy_back_type, &serial_number ) );
      m_logger << level << "Board information:\n"
               << "Type           : " << toString( m_type ) << "\n"
               << "Address        : "
               << std::hex << m_data->data()->m_common_data.m_base_address
               << "\n"
               << "Firmware rev.  : " << std::hex << firmware_rev << "\n"
               << "Piggy back type: "
               << cvt_V792_get_piggy_back_str( m_data->data(), piggy_back_type )
               << "\n"
               << "Serial number  : " << std::hex << serial_number
               << std::dec << msg::endmsg;
#else
      m_logger << level << tr( "Board info not available in simulation mode" )
               << msg::endmsg;
#endif // HAVE_CAEN_QTP_LIBS

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used during the initialisation of the device to
   /// clear all data from it, possibly remaining from a previous data
   /// acquisition session.
   ///
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::dataClear() {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Clear the device:
      CHECK( cvt_V792_data_clear( m_data->data() ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Reset the simulation counter:
      m_data->nEvents() = 0;

      // Tell the user what happened:
      m_logger << msg::DEBUG << tr( "Cleared the data from the device" )
               << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to set the zero suppression behaviour of
   /// devices that support "single range" zero suppression.
   ///
   /// @param enable Flag enabling/disabling zero suppression
   /// @param stepThreshold If set, the comparison is CNV_VALUE > THR_VALUEx2,
   ///                      otherwise it's CNV_VALUE > THR_VALUEx16
   /// @param thresholds Vector of thresholds, with one value per channel. Has
   ///                   to have exactly as many elements as many channels are
   ///                   in the hardware board.
   /// @returns The usual status codes
   ///
   StatusCode
   VmeDevice::setZeroSuppression( bool enable, bool stepThreshold,
                                  const std::vector< uint16_t >& thresholds ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Make sure that the size of the threshold vector is correct for the
      // device type. But not on Windows. Since the needed symbol can't be used
      // correctly there...
#ifndef WIN32
      const size_t requiredSize =
            CVT_QTP_NUM_CHANNELS[ m_data->data()->m_type ];
      CHECK( thresholds.size() == requiredSize );
#endif // WIN32
      // Call the setter function:
      CHECK( cvt_V792_set_zero_suppression( m_data->data(),
                                            enable, stepThreshold,
                                            thresholds.data() ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG << tr( "Set zero suppression with:\n" )
               << "enable       : " << enable << "\n"
               << "stepThreshold: " << stepThreshold << "\n"
               << "thresholds   : " << thresholds << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to set a number of properties on the device
   /// that control its data acquisition properties.
   ///
   /// @param slidingScaleEnable Enable/Disable the sliding scale feature
   /// @param zeroSuppressionEnable Enable/Disable the zero suppression feature
   /// @param overflowSuppressionEnable Enable/Disable the overflow suppression
   ///        feature
   /// @param validSuppressionEnable Enable/Disable the valid suppression
   ///        feature (V775 ONLY)
   /// @param commonStopEnable Enable/Disable the common stop acquisition mode
   ///        (V775 ONLY)
   /// @param emptyEnable Enable/Disable empty event storing feature
   /// @param countAllEvents Enable counting all triggers or accepted one only
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::setAcquisitionMode( bool slidingScaleEnable,
                                             bool zeroSuppressionEnable,
                                             bool overflowSuppressionEnable,
                                             bool validSuppressionEnable,
                                             bool commonStopEnable,
                                             bool emptyEnable,
                                             bool countAllEvents ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the setter function:
      CHECK( cvt_V792_set_acquisition_mode( m_data->data(),
                                            slidingScaleEnable,
                                            zeroSuppressionEnable,
                                            overflowSuppressionEnable,
                                            validSuppressionEnable,
                                            commonStopEnable,
                                            emptyEnable,
                                            countAllEvents ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG << tr( "Set acquisition mode with:\n" )
               << "slidingScaleEnable       : " << slidingScaleEnable << "\n"
               << "zeroSuppressionEnable    : " << zeroSuppressionEnable << "\n"
               << "overflowSuppressionEnable: " << overflowSuppressionEnable
               << "\n"
               << "validSuppressionEnable   : " << validSuppressionEnable
               << "\n"
               << "commonStopEnable         : " << commonStopEnable << "\n"
               << "emptyEnable              : " << emptyEnable << "\n"
               << "countAllEvents           : " << countAllEvents
               << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to configure the readout mode settings of the
   /// device.
   ///
   /// @param busErrorEnable Enable/disable bus error. The module is enabled to
   ///        generate a Bus error to finish a block transfer.
   /// @param blockEndEnable Enable/disable block end. the module sends all data
   ///        to the CPU until the first EOB word (end of first event) is
   ///        reached.
   /// @param align64Enable Enable/disable align 64 mode. The module is enabled
   ///        to add a dummy word when the number of words is odd.
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::setReadoutMode( bool busErrorEnable,
                                         bool blockEndEnable,
                                         bool align64Enable ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the setter function:
      CHECK( cvt_V792_set_readout_mode( m_data->data(), busErrorEnable,
                                        blockEndEnable, align64Enable ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG << tr( "Set readout mode with:\n" )
               << "busErrorEnable: " << busErrorEnable << "\n"
               << "blockEndEnable: " << blockEndEnable << "\n"
               << "align64Enable : " << align64Enable << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to set the pedestal value of the device.
   /// It is actually used in a fairly hacky way at the moment. Since for V775
   /// the Full Scale Range register is on the same address that the pedestal
   /// register uses on some other modules, it was easiest to just re-use this
   /// function for setting the value of that register.
   ///
   /// @param value Value to be set for the pedestal/FSR register
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::setPedestal( uint8_t value ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the setter function:
      CHECK( cvt_V792_set_pedestal( m_data->data(), value ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG
               << tr( "Set the pedestal to: %1" ).arg( value, 2, 16 )
               << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to write a particular value into the device's
   /// Bit Set 1 register. See the device's documentation for information on
   /// the meaning of the various bits.
   ///
   /// @param value The value to write into the register
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::writeBitSet1Register( uint16_t value ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the library function:
      CHECK( cvt_V792_set_bit_set_1( m_data->data(), value ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG
               << tr( "Wrote value (bin) %1 to the bit set 1 register" )
                  .arg( value, 16, 2 ) << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to write a particular value into the device's
   /// Bit Clear 1 register. See the device's documentation for information on
   /// the meaning of the various bits.
   ///
   /// @param value The value to write into the register
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::writeBitClear1Register( uint16_t value ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the library function:
      CHECK( cvt_V792_set_bit_clear_1( m_data->data(), value ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG
               << tr( "Wrote value (bin) %1 to the bit clear 1 register" )
                  .arg( value, 16, 2 ) << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to write a particular value into the device's
   /// Bit Set 2 register. See the device's documentation for information on
   /// the meaning of the various bits.
   ///
   /// @param value The value to write into the register
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::writeBitSet2Register( uint16_t value ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the library function:
      CHECK( cvt_V792_set_bit_set_2( m_data->data(), value ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG
               << tr( "Wrote value (bin) %1 to the bit set 2 register" )
                  .arg( value, 16, 2 ) << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to write a particular value into the device's
   /// Bit Clear 2 register. See the device's documentation for information on
   /// the meaning of the various bits.
   ///
   /// @param value The value to write into the register
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::writeBitClear2Register( uint16_t value ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Call the library function:
      CHECK( cvt_V792_set_bit_clear_2( m_data->data(), value ) );
#endif // HAVE_CAEN_QTP_LIBS

      // Tell the user what happened:
      m_logger << msg::DEBUG
               << tr( "Wrote value (bin) %1 to the bit clear 2 register" )
                  .arg( value, 16, 2 ) << msg::endmsg;

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function can be used to read data from the device. It makes use of
   /// the underlying QTP library's Multiple Event Buffer readout to first copy
   /// the data into the application's memory, and then decodes the information
   /// into an easily digestible data format.
   ///
   /// @param events Container filled by the function with new event data
   /// @returns The usual status codes
   ///
   StatusCode VmeDevice::read( std::vector< DataEvent >& events ) {

      // Make sure that the board is connected to:
      CHECK( isConnected() );

#ifdef HAVE_CAEN_QTP_LIBS
      // Read data from the device into the allocated buffer:
      m_data->bufferUsage() = 0;
      while( ! m_data->bufferUsage() ) {
         // Read the data:
         m_data->bufferUsage() = VmeDevicePrivateData::BUFFER_SIZE;
         CHECK( cvt_V792_read_MEB( m_data->data(), m_data->buffer(),
                                   &( m_data->bufferUsage() ) ) );
         // If the data acquisition is to be stopped, do so:
         if( g_stopAcquisition ) {
            REPORT_VERBOSE( tr( "Stopping acquisition" ) );
            return StatusCode::SUCCESS;
         }
         // If there is no data available, wait a bit more:
         if( ! m_data->bufferUsage() ) {
            common::SleepMin();
         }
      }
      // The current event object, used in the payload decoding:
      DataEvent currentEvent;
      // Loop over the buffer, translating its contents into DataEvent
      // objects:
      uint32_t* itr = reinterpret_cast< uint32_t* >( m_data->buffer() );
      while( m_data->bufferUsage() >= 4 ) {
         // The current data word:
         uint32_t data = *( itr++ );
         // Update the buffer usage:
         m_data->bufferUsage() -= 4;
         // Decode the data word:
         switch( data & CVT_QTP_DATA_TYPE_MSK ) {
         case CVT_QTP_HEADER:
            // A new event is started. Clear out the current event object,
            // and set its header.
            currentEvent.data.clear();
            currentEvent.footer.geo = 0;
            currentEvent.footer.eventCount = 0;
            // Now set the properties of its header:
            currentEvent.header.geo = CVT_QTP_GET_HDR_GEO( data );
            currentEvent.header.crate = CVT_QTP_GET_HDR_CRATE( data );
            currentEvent.header.channelCount = CVT_QTP_GET_HDR_CH_COUNT( data );
            break;
         case CVT_QTP_EOB:
            // This is the end of the event. Fill the footer of the object:
            currentEvent.footer.geo = CVT_QTP_GET_EOB_GEO( data );
            currentEvent.footer.eventCount =
                  CVT_QTP_GET_EOB_EVENT_COUNT( data );
            // And now add the event to the output:
            events.push_back( currentEvent );
            break;
         case CVT_QTP_DATUM:
            // This is a new data word. Add one to the current event:
            currentEvent.data.push_back( DataWord() );
            // And now set its properties:
            currentEvent.data.back().geo = ::decodeGeo( data, m_type );
            currentEvent.data.back().channel = ::decodeChannel( data, m_type );
            currentEvent.data.back().data = ::decodeData( data, m_type );
            currentEvent.data.back().underThreshold =
                  ::decodeUnderThreshold( data, m_type );
            currentEvent.data.back().overflow = ::decodeOverflow( data,
                                                                  m_type );
            break;
         case CVT_QTP_NOT_VALID_DATUM:
            // This is an invalid data word. Print a warning.
            m_logger << msg::WARNING << tr( "Incorrect data word found" )
                     << msg::endmsg;
            break;
         default:
            // This is an unknown word. This should not happen.
            REPORT_ERROR( tr( "Unknown data word found" ) );
            break;
         }
      }
      // Check that we read out all the data words:
      if( m_data->bufferUsage() > 0 ) {
         REPORT_ERROR( tr( "Data words were left in the MEB. This will lead "
                           "to data loss." ) );
      }
#else
      // Fill a small random number of events into the output:
      const int nevents = rand()  / ( RAND_MAX / 10 ) + 1;
      for( int i = 0; i < nevents; ++i ) {
         // Set the header and footer of the event:
         DataEvent event;
         event.header.geo = 0;
         event.header.crate = 0;
         event.header.channelCount = 32;
         event.footer.geo = 0;
         event.footer.eventCount = m_data->nEvents();
         // Generate some simple data values for it:
         for( int j = 0; j < 32; ++j ) {
            event.data.push_back( DataWord() );
            event.data.back().geo = 0;
            event.data.back().channel = j;
            event.data.back().data = ( j * 100 + 100 +
                                       ( rand() / ( RAND_MAX / 20 ) ) );
            event.data.back().underThreshold = 0;
            event.data.back().overflow = 0;
         }
         events.push_back( event );
         // Increment the simulated event count:
         ++( m_data->nEvents() );
         // With a low probability, simulate the device getting out of sync
         // with other devices:
         static const int unsyncProbability = RAND_MAX / 1000.0;
         if( rand() < unsyncProbability ) {
            ++( m_data->nEvents() );
         }
      }
      // Wait a moment before returning:
      common::Sleep( 100 );
#endif // HAVE_CAEN_QTP_LIBS

      // Return gracefully:
      return StatusCode::SUCCESS;
   }

   /// This function is used internally to make sure that we have a pristine
   /// private data object available when we connect to a new device.
   ///
   void VmeDevice::resetData() {

      if( m_data ) {
         delete m_data;
         m_data = 0;
      }
      m_data = new VmeDevicePrivateData();

      return;
   }

   /// This function tries to tell in a very simple way whether the object is
   /// connected to a hardware device at the moment.
   ///
   /// @returns <code>true</code> if the hardware is connected,
   ///          <code>false</code> otherwise
   ///
   bool VmeDevice::isConnected() const {

      return ( m_data != 0 );
   }

} // namespace caen
