// $Id$

// CAEN include(s):
#ifdef HAVE_CAEN_LIBS
// On Windows we compile the code with MinGW, so this definition would just
// get in the way:
#   undef WIN32
#   include <CAENDigitizer.h>
#endif // HAVE_CAEN_LIBS

// CDA include(s):
#include "../common/Sleep.h"

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

   /// Function turning a digitizer model into a human readable string
   const char* toString( CAEN_DGTZ_BoardModel_t model ) {

      switch( model ) {

      case CAEN_DGTZ_V1724:
         return "V1724";
         break;
      case CAEN_DGTZ_V1721:
         return "V1721";
         break;
      case CAEN_DGTZ_V1731:
         return "V1731";
         break;
      case CAEN_DGTZ_V1720:
         return "V1720";
         break;
      case CAEN_DGTZ_V1740:
         return "V1740";
         break;
      case CAEN_DGTZ_V1751:
         return "V1751";
         break;
      case CAEN_DGTZ_DT5724:
         return "DT5724";
         break;
      case CAEN_DGTZ_DT5721:
         return "DT5721";
         break;
      case CAEN_DGTZ_DT5731:
         return "DT5731";
         break;
      case CAEN_DGTZ_DT5720:
         return "DT5720";
         break;
      case CAEN_DGTZ_DT5740:
         return "DT5740";
         break;
      case CAEN_DGTZ_DT5751:
         return "DT5751";
         break;
      case CAEN_DGTZ_N6724:
         return "N6724";
         break;
      case CAEN_DGTZ_N6721:
         return "N6721";
         break;
      case CAEN_DGTZ_N6731:
         return "N6731";
         break;
      case CAEN_DGTZ_N6720:
         return "N6720";
         break;
      case CAEN_DGTZ_N6740:
         return "N6740";
         break;
      case CAEN_DGTZ_N6751:
         return "N6751";
         break;
      case CAEN_DGTZ_DT5742:
         return "DT5742";
         break;
      case CAEN_DGTZ_N6742:
         return "N6742";
         break;
      case CAEN_DGTZ_V1742:
         return "V1742";
         break;
      default:
         return "Board model not recognized";
         break;
      }
      return "Coding error detected";
   }

   /// Function turning a digitizer form factor into a human readable string
   const char* toString( CAEN_DGTZ_BoardFormFactor_t form ) {

      switch( form ) {

      case CAEN_DGTZ_VME64_FORM_FACTOR:
         return "VME64";
         break;
      case CAEN_DGTZ_VME64X_FORM_FACTOR:
         return "VME64X";
         break;
      case CAEN_DGTZ_DESKTOP_FORM_FACTOR:
         return "Desktop";
         break;
      case CAEN_DGTZ_NIM_FORM_FACTOR:
         return "NIM";
         break;
      default:
         return "Board form factor not recognized";
         break;
      }
      return "Coding error detected";
   }

   /// Convert between the connection types
   CAEN_DGTZ_ConnectionType convert( caen::Digitizer::ConnectionType type ) {

      switch( type ) {

      case caen::Digitizer::USB:
         return CAEN_DGTZ_USB;
         break;
      case caen::Digitizer::PCI_OpticalLink:
         return CAEN_DGTZ_PCI_OpticalLink;
         break;
      case caen::Digitizer::PCIE_OpticalLink:
         return CAEN_DGTZ_PCIE_OpticalLink;
         break;
      case caen::Digitizer::PCIE_EmbeddedDigitizer:
         return CAEN_DGTZ_PCIE_EmbeddedDigitizer;
         break;
      default:
         return static_cast< CAEN_DGTZ_ConnectionType >( 10L );
         break;
      }
      return static_cast< CAEN_DGTZ_ConnectionType >( 10L );
   }

   /// Convert between the trigger mode enumerations
   CAEN_DGTZ_TriggerMode_t convert( caen::Digitizer::TriggerMode mode ) {

      switch( mode ) {

      case caen::Digitizer::TRIG_Disabled:
         return CAEN_DGTZ_TRGMODE_DISABLED;
         break;
      case caen::Digitizer::TRIG_ExtOnly:
         return CAEN_DGTZ_TRGMODE_EXTOUT_ONLY;
         break;
      case caen::Digitizer::TRIG_AcqOnly:
         return CAEN_DGTZ_TRGMODE_ACQ_ONLY;
         break;
      case caen::Digitizer::TRIG_AcqAndExt:
         return CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT;
         break;
      default:
         return static_cast< CAEN_DGTZ_TriggerMode_t >( 10L );
         break;
      }
      return static_cast< CAEN_DGTZ_TriggerMode_t >( 10L );
   }

   /// Convert between the trigger mode enumerations
   caen::Digitizer::TriggerMode convert( CAEN_DGTZ_TriggerMode_t mode ) {

      switch( mode ) {

      case CAEN_DGTZ_TRGMODE_DISABLED:
         return caen::Digitizer::TRIG_Disabled;
         break;
      case CAEN_DGTZ_TRGMODE_EXTOUT_ONLY:
         return caen::Digitizer::TRIG_ExtOnly;
         break;
      case CAEN_DGTZ_TRGMODE_ACQ_ONLY:
         return caen::Digitizer::TRIG_AcqOnly;
         break;
      case CAEN_DGTZ_TRGMODE_ACQ_AND_EXTOUT:
         return caen::Digitizer::TRIG_AcqAndExt;
         break;
      default:
         return static_cast< caen::Digitizer::TriggerMode >( 10L );
         break;
      }
      return static_cast< caen::Digitizer::TriggerMode >( 10L );
   }

   /// Convert between the acquisition mode enumerations
   CAEN_DGTZ_AcqMode_t convert( caen::Digitizer::AcquisitionMode mode ) {

      switch( mode ) {

      case caen::Digitizer::ACQ_SW_Controlled:
         return CAEN_DGTZ_SW_CONTROLLED;
         break;
      case caen::Digitizer::ACQ_SIn_Controlled:
         return CAEN_DGTZ_S_IN_CONTROLLED;
         break;
      default:
         return static_cast< CAEN_DGTZ_AcqMode_t >( 10L );
         break;
      }
      return static_cast< CAEN_DGTZ_AcqMode_t >( 10L );
   }

   /// Convert between the acquisition mode enumerations
   caen::Digitizer::AcquisitionMode convert( CAEN_DGTZ_AcqMode_t mode ) {

      switch( mode ) {

      case CAEN_DGTZ_SW_CONTROLLED:
         return caen::Digitizer::ACQ_SW_Controlled;
         break;
      case CAEN_DGTZ_S_IN_CONTROLLED:
         return caen::Digitizer::ACQ_SIn_Controlled;
         break;
      default:
         return static_cast< caen::Digitizer::AcquisitionMode >( 10L );
         break;
      }
      return static_cast< caen::Digitizer::AcquisitionMode >( 10L );
   }

   /// Convert a channel number to a channel mask
   uint32_t toMask( uint32_t channel ) {

      return ( 0x1 << channel );
   }

   /// Convert between read mode enumerations
   CAEN_DGTZ_ReadMode_t convert( caen::Digitizer::ReadMode mode ) {

      switch( mode ) {

      case caen::Digitizer::READ_SlaveTerminatedMBLT:
         return CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT;
         break;
      case caen::Digitizer::READ_SlaveTerminated2eVME:
         return CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eVME;
         break;
      case caen::Digitizer::READ_SlaveTerminated2eSST:
         return CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eSST;
         break;
      case caen::Digitizer::READ_PollingMBLT:
         return CAEN_DGTZ_POLLING_MBLT;
         break;
      case caen::Digitizer::READ_Polling2eVME:
         return CAEN_DGTZ_POLLING_2eVME;
         break;
      case caen::Digitizer::READ_Polling2eSST:
         return CAEN_DGTZ_POLLING_2eSST;
         break;
      default:
         return static_cast< CAEN_DGTZ_ReadMode_t >( 10L );
         break;
      }
      return static_cast< CAEN_DGTZ_ReadMode_t >( 10L );
   }

   /// Convert read mode into human readable format
   const char* toString( caen::Digitizer::ReadMode mode ) {

      switch( mode ) {

      case caen::Digitizer::READ_SlaveTerminatedMBLT:
         return "CAEN_DGTZ_SLAVE_TERMINATED_READOUT_MBLT";
         break;
      case caen::Digitizer::READ_SlaveTerminated2eVME:
         return "CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eVME";
         break;
      case caen::Digitizer::READ_SlaveTerminated2eSST:
         return "CAEN_DGTZ_SLAVE_TERMINATED_READOUT_2eSST";
         break;
      case caen::Digitizer::READ_PollingMBLT:
         return "CAEN_DGTZ_POLLING_MBLT";
         break;
      case caen::Digitizer::READ_Polling2eVME:
         return "CAEN_DGTZ_POLLING_2eVME";
         break;
      case caen::Digitizer::READ_Polling2eSST:
         return "CAEN_DGTZ_POLLING_2eSST";
         break;
      default:
         return "Unknown";
         break;
      }
      return "Unknown";
   }

} // private namespace

/// Macro checking the return value of a CAEN Digitizer function
#define CHECK( CMD ) {                                                  \
      CAEN_DGTZ_ErrorCode code = CMD;                                   \
      if( code != CAEN_DGTZ_Success ) {                                 \
         REPORT_ERROR( tr( "Failed executing \"%1\", "                  \
                           "Return value: %2" ).arg( #CMD )             \
                       .arg( toString( code ) ) );                      \
         return false;                                                  \
      }                                                                 \
   } while( 0 ){}

#endif // HAVE_CAEN_LIBS

namespace caen {

   Digitizer::Digitizer()
      : m_handle( -1 ),
        m_logger( "caen::Digitizer" ) {

   }

   Digitizer::~Digitizer() {

      close();
   }

   int Digitizer::convertConnType( ConnectionType type ) {

      switch( type ) {

      case USB:
         return 0;
         break;
      case PCI_OpticalLink:
         return 1;
         break;
      case PCIE_OpticalLink:
         return 2;
         break;
      case PCIE_EmbeddedDigitizer:
         return 3;
         break;
      default:
         return 0;
         break;
      }

      return 1000;
   }

   Digitizer::ConnectionType Digitizer::convertConnType( int type ) {

      switch( type ) {

      case 0:
         return USB;
         break;
      case 1:
         return PCI_OpticalLink;
         break;
      case 2:
         return PCIE_OpticalLink;
         break;
      case 3:
         return PCIE_EmbeddedDigitizer;
         break;
      default:
         return USB;
         break;
      }

      return static_cast< ConnectionType >( 1000 ); 
   }

   /**
    * This function opens the connection to the digitizer using
    * the CAEN Digitizer library. While the library supports
    * a wide variety of devices, the class can currently only
    * be tested in USB mode. (Only the CAEN DT5740 will be
    * tested in the foreseeable future.)
    *
    * @param type Connection type
    * @param linkNum Link number (0 when only one USB device)
    * @param conetNode ID of the device in dasy chain (0 for USB)
    * @param vmeAddress VME base address (0 for USB)
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> if it wasn't
    */
   bool Digitizer::open( ConnectionType type, int linkNum,
                         int conetNode, uint32_t vmeAddress ) {

      // Open the connection:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_OpenDigitizer( convert( type ), linkNum, conetNode,
                                      vmeAddress, &m_handle ) );
#else
      REPORT_VERBOSE( tr( "Opening connection type %1 on link number %2, "
                          " CONET node %3, VME address %4" )
                      .arg( convertConnType( type ) ).arg( linkNum )
                      .arg( conetNode ).arg( vmeAddress ) );
#endif // HAVE_CAEN_LIBS

      // Let the user know what we did:
      m_logger << msg::DEBUG
               << tr( "Opened connection to digitizer" )
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
               << tr( "Closed connection to digitizer" )
               << msg::endmsg;

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::writeRegister( uint32_t address, uint32_t data ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_WriteRegister( m_handle, address, data ) );
#else
      m_logger << msg::DEBUG
               << tr( "writeRegister( address: %1, data: %2 )" )
         .arg( address ).arg( data )
               << msg::endmsg;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::readRegister( uint32_t address,
                                 uint32_t& data ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_ReadRegister( m_handle, address, &data ) );
#else
      m_logger << msg::DEBUG
               << tr( "readRegister( address: %1, data: to be set )" )
         .arg( address )
               << msg::endmsg;
      data = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   /**
    * This function can be used to print some information about the
    * digitizer that this object is connected to.
    *
    * @param level Message level of the printed information
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> if it wasn't
    */
   bool Digitizer::printInfo( msg::Level level ) const {

#ifdef HAVE_CAEN_LIBS
      // Read the board information:
      CAEN_DGTZ_BoardInfo_t boardInfo;
      CHECK( CAEN_DGTZ_GetInfo( m_handle, &boardInfo ) );
      // Now print it:
      m_logger << level
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
         .arg( boardInfo.ModelName )
         .arg( toString( static_cast< CAEN_DGTZ_BoardModel_t >( boardInfo.Model ) ) )
         .arg( boardInfo.Channels )
         .arg( toString( static_cast< CAEN_DGTZ_BoardFormFactor_t >( boardInfo.FormFactor ) ) )
         .arg( boardInfo.FamilyCode ).arg( boardInfo.ROC_FirmwareRel )
         .arg( boardInfo.AMC_FirmwareRel ).arg( boardInfo.SerialNumber )
         .arg( boardInfo.PCB_Revision ).arg( boardInfo.ADC_NBits )
               << msg::endmsg;
#else
      m_logger << level
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

   int Digitizer::convertAcqMode( Digitizer::AcquisitionMode mode ) {

      switch( mode ) {

      case ACQ_SW_Controlled:
         return 0;
         break;
      case ACQ_SIn_Controlled:
         return 1;
         break;
      default:
         return 0;
         break;
      }

      return 1000;
   }

   Digitizer::AcquisitionMode Digitizer::convertAcqMode( int mode ) {

      switch( mode ) {

      case 0:
         return ACQ_SW_Controlled;
         break;
      case 1:
         return ACQ_SIn_Controlled;
         break;
      default:
         return ACQ_SW_Controlled;
         break;
      }

      return static_cast< AcquisitionMode >( 1000 );
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

   bool Digitizer::setAcquisitionMode( AcquisitionMode mode ) {

      // Set the acquisition mode of the digitizer
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetAcquisitionMode( m_handle,
                                           convert( mode ) ) );
#else
      REPORT_VERBOSE( tr( "Setting acquisition mode to %1" )
                      .arg( mode ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getAcquisitionMode( AcquisitionMode& mode ) const {

      // Set the acquisition mode of the digitizer
#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_AcqMode_t cmode;
      CHECK( CAEN_DGTZ_GetAcquisitionMode( m_handle,
                                           &cmode ) );
      mode = convert( cmode );
#else
      mode = ACQ_SW_Controlled;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setInterruptAfterEvents( uint16_t event_number ) {

      // Set up interrupts on the digitizer:
#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetInterruptConfig( m_handle, CAEN_DGTZ_DISABLE,
                                           0, 0, event_number,
                                           CAEN_DGTZ_IRQ_MODE_RORA ) );
#else
      REPORT_VERBOSE( tr( "Setting interrupt to happen after %1 events" )
                      .arg( event_number ) );
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
      common::Sleep( timeout );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setSWTriggerMode( TriggerMode mode ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetSWTriggerMode( m_handle, convert( mode ) ) );
#else
      REPORT_VERBOSE( tr( "Setting SW trigger mode to %1" )
                      .arg( mode ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getSWTriggerMode( TriggerMode& mode ) const {

#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_TriggerMode_t cmode;
      CHECK( CAEN_DGTZ_GetSWTriggerMode( m_handle, &cmode ) );
      mode = convert( cmode );
#else
      mode = TRIG_Disabled;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setExtTriggerMode( TriggerMode mode ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetExtTriggerInputMode( m_handle,
                                               convert( mode ) ) );
#else
      REPORT_VERBOSE( tr( "Setting Ext trigger mode to %1" )
                      .arg( mode ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getExtTriggerMode( TriggerMode& mode ) const {

#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_TriggerMode_t cmode;
      CHECK( CAEN_DGTZ_GetExtTriggerInputMode( m_handle, &cmode ) );
      mode = convert( cmode );
#else
      mode = TRIG_Disabled;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setChannelSelfTriggerMode( uint32_t channel,
                                              TriggerMode mode ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetChannelSelfTrigger( m_handle, convert( mode ),
                                              toMask( channel ) ) );
#else
      REPORT_VERBOSE( tr( "Setting channel %1 self-trigger mode to %2" )
                      .arg( channel ).arg( mode ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getChannelSelfTriggerMode( uint32_t channel,
                                              TriggerMode& mode ) const {

#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_TriggerMode_t cmode;
      CHECK( CAEN_DGTZ_GetChannelSelfTrigger( m_handle, channel,
                                              &cmode ) );
      mode = convert( cmode );
#else
      REPORT_VERBOSE( tr( "Requesting channel %1's self-trigger mode" )
                      .arg( channel ) );
      mode = TRIG_Disabled;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setGroupSelfTriggerMode( uint32_t group,
                                            TriggerMode mode ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetGroupSelfTrigger( m_handle, convert( mode ),
                                            toMask( group ) ) );
#else
      REPORT_VERBOSE( tr( "Setting group %1 self-trigger mode to %2" )
                      .arg( group ).arg( mode ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getGroupSelfTriggerMode( uint32_t group,
                                            TriggerMode& mode ) const {

#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_TriggerMode_t cmode;
      CHECK( CAEN_DGTZ_GetGroupSelfTrigger( m_handle, group,
                                            &cmode ) );
      mode = convert( cmode );
#else
      REPORT_VERBOSE( tr( "Requesting group %1's self-trigger mode" )
                      .arg( group ) );
      mode = TRIG_Disabled;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setChannelGroupMask( uint32_t group,
                                        uint32_t mask ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetChannelGroupMask( m_handle, group,
                                            mask ) );
#else
      REPORT_VERBOSE( tr( "Setting group %1's channel mask to %2" )
                      .arg( group ).arg( mask ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getChannelGroupMask( uint32_t group,
                                        uint32_t& mask ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetChannelGroupMask( m_handle, group,
                                            &mask ) );
#else
      REPORT_VERBOSE( tr( "Requestuing group %1's channel mask" )
                      .arg( group ) );
      mask = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setChannelTriggerThreshold( uint32_t channel,
                                               uint32_t thr ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetChannelTriggerThreshold( m_handle, channel,
                                                   thr ) );
#else
      REPORT_VERBOSE( tr( "Setting channel %1's trigger threshold to %2" )
                      .arg( channel ).arg( thr ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getChannelTriggerThreshold( uint32_t channel,
                                               uint32_t& thr ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetChannelTriggerThreshold( m_handle, channel,
                                                   &thr ) );
#else
      REPORT_VERBOSE( tr( "Retrieving trigger threshold for channel %1" )
                      .arg( channel ) );
      thr = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setGroupTriggerThreshold( uint32_t group,
                                             uint32_t thr ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetGroupTriggerThreshold( m_handle, group,
                                                 thr ) );
#else
      REPORT_VERBOSE( tr( "Setting group %1's trigger threshold to %2" )
                      .arg( group ).arg( thr ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getGroupTriggerThreshold( uint32_t group,
                                             uint32_t& thr ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetGroupTriggerThreshold( m_handle, group,
                                                 &thr ) );
#else
      REPORT_VERBOSE( tr( "Retrieving trigger threshold for group %1" )
                      .arg( group ) );
      thr = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setChannelEnableMask( uint32_t mask ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetChannelEnableMask( m_handle, mask ) );
#else
      REPORT_VERBOSE( tr( "Channel enable mask set to %1" )
                      .arg( mask ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getChannelEnableMask( uint32_t& mask ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetChannelEnableMask( m_handle, &mask ) );
#else
      mask = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setGroupEnableMask( uint32_t mask ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetGroupEnableMask( m_handle, mask ) );
#else
      REPORT_VERBOSE( tr( "Group enable mask set to %1" )
                      .arg( mask ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getGroupEnableMask( uint32_t& mask ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetGroupEnableMask( m_handle, &mask ) );
#else
      mask = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setRecordLength( uint32_t size ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetRecordLength( m_handle, size ) );
#else
      REPORT_VERBOSE( tr( "Record length set to %1" )
                      .arg( size ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getRecordLength( uint32_t& size ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetRecordLength( m_handle, &size ) );
#else
      size = 100;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setPostTriggerSize( uint32_t percent ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetPostTriggerSize( m_handle, percent ) );
#else
      REPORT_VERBOSE( tr( "Post trigger size set to %1\%" )
                      .arg( percent ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getPostTriggerSize( uint32_t& percent ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetPostTriggerSize( m_handle, &percent ) );
#else
      percent = 100;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setChannelDCOffset( uint32_t channel,
                                       uint32_t value ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetChannelDCOffset( m_handle, channel,
                                           value ) );
#else
      REPORT_VERBOSE( tr( "Setting channel %1's DC offset to %2" )
                      .arg( channel ).arg( value ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getChannelDCOffset( uint32_t channel,
                                       uint32_t& value ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetChannelDCOffset( m_handle, channel,
                                           &value ) );
#else
      REPORT_VERBOSE( tr( "Retrieving channel %1's DC offset" )
                      .arg( channel ) );
      value = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setGroupDCOffset( uint32_t group,
                                     uint32_t value ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetGroupDCOffset( m_handle, group,
                                         value ) );
#else
      REPORT_VERBOSE( tr( "Setting group %1's DC offset to %2" )
                      .arg( group ).arg( value ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getGroupDCOffset( uint32_t group,
                                     uint32_t& value ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetGroupDCOffset( m_handle, group,
                                         &value ) );
#else
      REPORT_VERBOSE( tr( "Retrieving group %1's DC offset" )
                      .arg( group ) );
      value = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::setMaxNumEventsBLT( uint32_t events ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_SetMaxNumEventsBLT( m_handle, events ) );
#else
      REPORT_VERBOSE( tr( "Setting maximum number of events to read out to %1" )
                      .arg( events ) );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getMaxNumEventsBLT( uint32_t& events ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetMaxNumEventsBLT( m_handle, &events ) );
#else
      REPORT_VERBOSE( tr( "Retrieving maximum number of events to read out" ) );
      events = 0;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::mallocReadoutBuffer( char** buffer, uint32_t& size ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_MallocReadoutBuffer( m_handle, buffer, &size ) );
#else
      REPORT_VERBOSE( tr( "Allocating readout buffer" ) );
      ( *buffer ) = new char[ 100 ];
      size = 100;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::freeReadoutBuffer( char** buffer ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_FreeReadoutBuffer( buffer ) );
#else
      REPORT_VERBOSE( tr( "Freeing readout buffer" ) );
      delete[] ( *buffer );
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::readData( ReadMode mode, char* buffer,
                             uint32_t& bufferSize ) {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_ReadData( m_handle, convert( mode ),
                                 buffer, &bufferSize ) );
#else
      REPORT_VERBOSE( tr( "Reading data in mode \"%1\"" )
                      .arg( toString( mode ) ) );
      *buffer = 0xf5;
      bufferSize = 1;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getNumEvents( char* buffer, uint32_t bufferSize,
                                 uint32_t& numEvents ) const {

#ifdef HAVE_CAEN_LIBS
      CHECK( CAEN_DGTZ_GetNumEvents( m_handle, buffer,
                                     bufferSize, &numEvents ) );
#else
      REPORT_VERBOSE( tr( "Extracting number of events from buffer of size %1" )
                      .arg( bufferSize ) );
      numEvents = 1;
#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getEvent( char* buffer, uint32_t bufferSize,
                             int32_t event, EventInfo& eventInfo,
                             EventData16Bit& eventData ) {

#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_EventInfo_t ei;
      char* evtPtr = NULL;
      CAEN_DGTZ_UINT16_EVENT_t* evt = NULL;
      void* evtVoidPtr = evt;
      // Get the information about this event:
      CHECK( CAEN_DGTZ_GetEventInfo( m_handle, buffer, bufferSize,
                                     event, &ei, &evtPtr ) );
      // Fill the event info into the output:
      eventInfo.eventSize      = ei.EventSize;
      eventInfo.boardId        = ei.BoardId;
      eventInfo.pattern        = ei.Pattern;
      eventInfo.channelMask    = ei.ChannelMask;
      eventInfo.eventCounter   = ei.EventCounter;
      eventInfo.triggerTimeTag = ei.TriggerTimeTag;
      // Decode the event:
      CHECK( CAEN_DGTZ_DecodeEvent( m_handle, evtPtr,
                                    &evtVoidPtr ) );
      // Fill the event data to the output:
      for( int i = 0; i < EventData16Bit::MAX_CHANNEL_NUMBER; ++i ) {
         eventData.chSize[ i ] = evt->ChSize[ i ];
         eventData.chData[ i ].resize( eventData.chSize[ i ], 0 );
         for( uint32_t j = 0; j < eventData.chSize[ i ]; ++j ) {
            eventData.chData[ i ][ j ] = evt->DataChannel[ i ][ j ];
         }
      }
      // Free the allocated memory:
      CHECK( CAEN_DGTZ_FreeEvent( m_handle, &evtVoidPtr ) );
#else

#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

   bool Digitizer::getEvent( char* buffer, uint32_t bufferSize,
                             int32_t event, EventInfo& eventInfo,
                             EventData8Bit& eventData ) {

#ifdef HAVE_CAEN_LIBS
      CAEN_DGTZ_EventInfo_t ei;
      char* evtPtr = NULL;
      CAEN_DGTZ_UINT8_EVENT_t* evt = NULL;
      void* evtVoidPtr = evt;
      // Get the information about this event:
      CHECK( CAEN_DGTZ_GetEventInfo( m_handle, buffer, bufferSize,
                                     event, &ei, &evtPtr ) );
      // Fill the event info into the output:
      eventInfo.eventSize      = ei.EventSize;
      eventInfo.boardId        = ei.BoardId;
      eventInfo.pattern        = ei.Pattern;
      eventInfo.channelMask    = ei.ChannelMask;
      eventInfo.eventCounter   = ei.EventCounter;
      eventInfo.triggerTimeTag = ei.TriggerTimeTag;
      // Decode the event:
      CHECK( CAEN_DGTZ_DecodeEvent( m_handle, evtPtr,
                                    &evtVoidPtr ) );
      // Fill the event data to the output:
      for( int i = 0; i < EventData8Bit::MAX_CHANNEL_NUMBER; ++i ) {
         eventData.chSize[ i ] = evt->ChSize[ i ];
         eventData.chData[ i ].resize( eventData.chSize[ i ], 0 );
         for( uint32_t j = 0; j < eventData.chSize[ i ]; ++j ) {
            eventData.chData[ i ][ j ] = evt->DataChannel[ i ][ j ];
         }
      }
      // Free the allocated memory:
      CHECK( CAEN_DGTZ_FreeEvent( m_handle, &evtVoidPtr ) );
#else

#endif // HAVE_CAEN_LIBS

      // Signal a successful operation:
      return true;
   }

} // namespace caen
