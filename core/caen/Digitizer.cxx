// $Id$

// System include(s):
extern "C" {
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
}

// CAEN include(s):
#ifdef HAVE_CAEN_LIBS
#   include <CAENDigitizer.h>
#endif // HAVE_CAEN_LIBS

// Local include(s):
#include "Digitizer.h"

#ifdef HAVE_CAEN_LIBS
namespace {
   /// Function turning a CAEN error code into a string
   /**
    * To print meaningful error messages easily, this function
    * can be used to print information about the error code returned
    * by a CAEN Digitizer function.
    *
    * @param code A CAEN Digitizer error code
    * @returns A human readable description of the error
    */
   const char* toString( CAEN_DGTZ_ErrorCode code ) {

      switch( code ) {

      case CAEN_DGTZ_Success:
         return "Success: Operation completed successfully";
         break;
      case CAEN_DGTZ_CommError:
         return "CommError: Communication error";
         break;
      case CAEN_DGTZ_GenericError:
         return "GenericError: Unspecified error";
         break;
      case CAEN_DGTZ_InvalidParam:
         return "InvalidParam: Invalid parameter";
         break;
      case CAEN_DGTZ_InvalidLinkType:
         return "InvalidLinkType: Invalid Link Type";
         break;
      case CAEN_DGTZ_InvalidHandle:
         return "InvalidHandle: Invalid device handle";
         break;
      case CAEN_DGTZ_MaxDevicesError:
         return "MaxDevicesError: Maximum number of devices exceeded";
         break;
      case CAEN_DGTZ_BadBoardType:
         return "BadBoardType: The operation is not allowed on this "
            "type of board";
         break;
      case CAEN_DGTZ_BadInterruptLev:
         return "BadInterruptLev: The interrupt level is not allowed";
         break;
      case CAEN_DGTZ_BadEventNumber:
         return "BadEventNumber: The event number is bad";
         break;
      case CAEN_DGTZ_ReadDeviceRegisterFail:
         return "ReadDeviceRegisterFail: Unable to read the register";
         break;
      case CAEN_DGTZ_WriteDeviceRegisterFail:
         return "WriteDeviceRegisterFail: Unable to write the register";
         break;
      case CAEN_DGTZ_InvalidChannelNumber:
         return "InvalidChannelNumber: The channel number is invalid";
         break;
      case CAEN_DGTZ_ChannelBusy:
         return "ChannelBusy: The Channel is busy";
         break;
      case CAEN_DGTZ_FPIOModeInvalid:
         return "FPIOModeInvalid: Invalid FPIO Mode";
         break;
      case CAEN_DGTZ_WrongAcqMode:
         return "WrongAcqMode: Wrong acquisition mode";
         break;
      case CAEN_DGTZ_FunctionNotAllowed:
         return "FunctionNotAllowed: This function is not allowed for "
            "this module";
         break;
      case CAEN_DGTZ_Timeout:
         return "Timeout: Communication Timeout";
         break;
      case CAEN_DGTZ_InvalidBuffer:
         return "InvalidBuffer: The buffer is invalid";
         break;
      case CAEN_DGTZ_EventNotFound:
         return "EventNotFound: The event is not found";
         break;
      case CAEN_DGTZ_InvalidEvent:
         return "InvalidEvent: The event is invalid";
         break;
      case CAEN_DGTZ_OutOfMemory:
         return "OutOfMemory: Out of memory";
         break;
      case CAEN_DGTZ_CalibrationError:
         return "CalibrationError: Unable to calibrate the board";
         break;
      case CAEN_DGTZ_DigitizerNotFound:
         return "DigitizerNotFound: Unable to open the digitizer";
         break;
      case CAEN_DGTZ_DigitizerAlreadyOpen:
         return "DigitizerAlreadyOpen: The Digitizer is already open";
         break;
      case CAEN_DGTZ_DigitizerNotReady:
         return "DigitizerNotReady: The Digitizer is not ready to operate";
         break;
      case CAEN_DGTZ_InterruptNotConfigured:
         return "InterruptNotConfigured: The Digitizer doesn't have the "
            "IRQ configured";
         break;
      case CAEN_DGTZ_DigitizerMemoryCorrupted:
         return "DigitizerMemoryCorrupted: The digitizer flash memory "
            "is corrupted";
         break;
      case CAEN_DGTZ_NotYetImplemented:
         return "NotYetImplemented: The function is not yet implemented";
         break;
      default:
         return "Error code not recognized";
         break;
      }
      return "Coding error detected";
   }
} // private namespace
/// Define the name of the current function
#if defined( __GNUC__ )
#   define CHECK_FNAME __PRETTY_FUNCTION__
#else
#   define CHECK_FNAME ""
#endif
/// Prefix put before error messages
#define CHECK_REPORT_PREFIX \
   __FILE__ << ":" << __LINE__ << " (" << CHECK_FNAME << "): "
/// Macro checking the return value of a CAEN Digitizer function
#define CHECK(CMD) {                                                    \
      CAEN_DGTZ_ErrorCode code = CMD;                                   \
      if( code != CAEN_DGTZ_Success ) {                                 \
         m_logger << msg::ERROR                                         \
                  << CHECK_REPORT_PREFIX                                \
                  << tr( "Failed executing \"%1\", "                    \
                         "Return value: %2" ).arg( #CMD )               \
            .arg( toString( code ) )                                    \
                  << msg::endmsg;                                       \
         return false;                                                  \
      }                                                                 \
   } while( 0 )
#endif // HAVE_CAEN_LIBS

namespace caen {

   Digitizer::Digitizer()
      : m_handle( -1 ), m_interruptAfterEvents( 0 ),
        m_logger( "caen::Digitizer" ) {

   }

   Digitizer::~Digitizer() {

      close();
   }

   /**
    * This function opens the connection to the digitizer using
    * the CAEN Digitizer library. While the library supports
    * a wide variety of devices, the class currently only supports
    * connecting to an USB device. (Only the CAEN DT5740 will be
    * tested in the foreseeable future.)
    *
    * @param type Connection type (Only the default USB connection
    *             is supported)
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> if it wasn't
    */
   bool Digitizer::open( ConnectionType type ) {

      // The code only supports the USB option for the moment:
      if( type != USB ) {
         m_logger << msg::ERROR
                  << tr( "Only the USB connection mode is supported "
                         "at the moment" )
                  << msg::endmsg;
         return false;
      }

      // Open the connection:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_OpenDigitizer( CAEN_DGTZ_USB, 0, 0, 0,
                                      &m_handle ) );
#endif // HAVE_CAEN_LIBS

      // Let the user know what we did:
      m_logger << msg::DEBUG
               << tr( "Opened USB connection" )
               << msg::endmsg;

      // Signal a successful operation:
      return true;
   }

   /**
    * This function closes the connetion to the device(s).
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> if it wasn't
    */
   bool Digitizer::close() {

      // Check if the device is even open:
      if( m_handle == -1 ) {
         m_logger << msg::DEBUG
                  << tr( "No need to close the device, it's not open" )
                  << msg::endmsg;
         return true;
      }

      // Close the connection:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_CloseDigitizer( m_handle ) );
#endif // HAVE_CAEN_LIBS

      // Reset the device handle:
      m_handle = 0;

      // Let the user know what we did:
      m_logger << msg::DEBUG
               << tr( "Closed USB connection" )
               << msg::endmsg;

      // Signal a successful operation:
      return true;
   }

   /**
    * This function can be used to print some information about the
    * digitizer that this object is connected to.
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> if it wasn't
    */
   bool Digitizer::printInfo() const {

#ifdef HAVE_CAEN_LIBS
      // Read the board information:
      CAEN_DGTZ_BoardInfo_t boardInfo;
      CHECK( CAEN_DGTZ_GetInfo( m_handle, &boardInfo ) );
      // Now print it:
      m_logger << msg::INFO
               << tr( "Digitizer board information:\n"
                      "  model name : %1\n"
                      "  model      : %2\n"
                      "  channels   : %3\n"
                      "  form factor: %4\n"
                      "  family code: %5\n"
                      "  ROC firmwate release: %6\n"
                      "  AMC firmware release: %7\n"
                      "  serial num.: %8\n"
                      "  PCB rev.   : %9\n"
                      "  ADC NBits  : %10" )
         .arg( boardInfo.ModelName ).arg( boardInfo.Model )
         .arg( boardInfo.Channels ).arg( boardInfo.FormFactor )
         .arg( boardInfo.FamilyCode ).arg( boardInfo.ROC_FirmwareRel )
         .arg( boardInfo.AMC_FirmwareRel ).arg( boardInfo.SerialNumber )
         .arg( boardInfo.PCB_Revision ).arg( boardInfo.ADC_NBits )
               << msg::endmsg;
#else
      m_logger << msg::INFO
               << tr( "No CAEN library available" )
               << msg::endmsg;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::reset() {

      // Reset the digitizer:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_Reset( m_handle ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::clear() {

      // Clear the data from the digitizer:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_ClearData( m_handle ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::startAcquisition() {

      // Start the data acquisition in the digitizer:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SWStartAcquisition( m_handle ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::stopAcquisition() {

      // Stop the data acquisition in the digitizer:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SWStopAcquisition( m_handle ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setInterruptAfterEvents( uint16_t event_number ) {

      // Remember the setting:
      m_interruptAfterEvents = event_number;

      // Set up interrupts on the digitizer:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetInterruptConfig( m_handle, CAEN_DGTZ_DISABLE,
                                           0, 0, event_number,
                                           CAEN_DGTZ_IRQ_MODE_RORA ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::printInterruptConfig() const {

#ifdef HAVE_CAEN_LIBS
      // Read the requested information:
      CAEN_DGTZ_EnaDis_t state;
      uint8_t level;
      uint32_t status_id;
      uint16_t event_number;
      CAEN_DGTZ_IRQMode_t mode;
      CHECK( CAEN_DGTZ_GetInterruptConfig( m_handle, &state, &level,
                                           &status_id, &event_number,
                                           &mode ) );
      // Now print it:
      m_logger << msg::INFO
               << tr( "Interrupt configuration:\n"
                      "  state       : %1\n"
                      "  level       : %2\n"
                      "  status ID   : %3\n"
                      "  event number: %4\n"
                      "  mode        : %5" )
         .arg( state == CAEN_DGTZ_ENABLE ? "ENABLED" : "DISABLED" )
         .arg( level ).arg( status_id ).arg( event_number )
         .arg( mode == CAEN_DGTZ_IRQ_MODE_RORA ? "RORA" : "ROAK" )
               << msg::endmsg;
#else
      m_logger << msg::INFO
               << tr( "No CAEN library available" )
               << msg::endmsg;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::irqWait( uint32_t timeout ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_IRQWait( m_handle, timeout ) );
#else
      // Wait for 100 miliseconds:
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 100000;
      select( 0, NULL, NULL, NULL, &tv );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

} // namespace caen
