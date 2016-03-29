// $Id$

// System include(s):
#include <cstring>

// CAEN include(s):
#ifdef HAVE_CAEN_QTP_LIBS
#   include <cvt_board_commons.h>
#   include <cvt_common_defs.h>
#   include <cvt_V792.h>
#endif

// CDA include(s):
#include "../msg/Logger.h"
#include "../common/errorcheck.h"

// Local include(s):
#include "VmeDevice.h"
#include "VmeBus.h"

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
         case caen::VmeDevice::DEV_V862A:
            return CVT_V862_TYPE_A;
            break;
         default:
            msg::Logger m_logger( "convert" );
            REPORT_ERROR( "Unknown device type received: " << type );
            return static_cast< CVT_QTP_TYPES >( 0 );
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
         case caen::VmeDevice::DEV_V862A:
            return "V862A";
            break;
         default:
            return "<unknown>";
      }
   }

} // private namespace

namespace caen {

   /// Helper class holding on to data used by the QTP library
   class VmeDevicePrivateData {

   public:
      /// Default constructor
      VmeDevicePrivateData() {
#ifdef HAVE_CAEN_QTP_LIBS
         // Make sure that the data object is zeroed out at the start
         memset( &m_data, 0, sizeof( m_data ) );
#endif // HAVE_CAEN_QTP_LIBS
      }

#ifdef HAVE_CAEN_QTP_LIBS
      /// Accessor for the data object
      cvt_V792_data* data() { return &m_data; }

   private:
      /// Data object used by the QTP library
      cvt_V792_data m_data;
#endif // HAVE_CAEN_QTP_LIBS

   }; // class VmeDevicePrivateData

   VmeDevice::VmeDevice()
      : m_data( 0 ), m_logger( "caen::VmeDevice" ) {

   }

   VmeDevice::~VmeDevice() {

      if( m_data ) {
         delete m_data;
      }
   }

   bool VmeDevice::open( uint16_t address, DeviceType type,
                         const VmeBus& bus ) {

      // Make sure that the data is cleared:
      resetData();

      // Open the connection to the device:
#ifdef HAVE_CAEN_QTP_LIBS
      CHECK( cvt_V792_open( m_data->data(), address, bus.handle(),
                            ::convert( type ) ) );
#endif // HAVE_CAEN_QTP_LIBS
      REPORT_VERBOSE( tr( "Device \"%1\" opened on address %2" )
                      .arg( toString( type ) ).arg( address, 4, 16 ) );

      // Return gracefully:
      return true;
   }

   bool VmeDevice::close() {

      // Make sure that we have a data object:
      if( ! m_data ) {
         m_logger << msg::WARNING << tr( "Trying to close a closed device" )
                  << msg::endmsg;
         return false;
      }

      // Close the connection to the device:
#ifdef HAVE_CAEN_QTP_LIBS
      CHECK( cvt_V792_close( m_data->data() ) );
#endif // HAVE_CAEN_QTP_LIBS
      REPORT_VERBOSE( tr( "Device closed" ) );

      // Return gracefully:
      return true;
   }

   void VmeDevice::resetData() {

      if( m_data ) {
         delete m_data;
         m_data = 0;
      }
      m_data = new VmeDevicePrivateData();

      return;
   }

} // namespace caen
