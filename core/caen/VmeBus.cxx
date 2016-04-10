// $Id$

// CAEN include(s):
#ifdef HAVE_CAEN_VME_LIBS
#   include <CAENVMElib.h>
#endif

// Local include(s):
#include "VmeBus.h"

#ifdef HAVE_CAEN_VME_LIBS
namespace {

   /// Convert from the custom type to the CAENVMELib one
   CVBoardTypes convert( caen::VmeBus::BoardType type ) {
      switch( type )
         {
         case caen::VmeBus::BOARD_V1718:
            return cvV1718;
            break;
         case caen::VmeBus::BOARD_V2718:
            return cvV2718;
            break;
         case caen::VmeBus::BOARD_A2818:
            return cvA2818;
            break;
         case caen::VmeBus::BOARD_A2719:
            return cvA2719;
            break;
         default:
            return static_cast< CVBoardTypes >( -1 );
            break;
         }
   }

   /// Return the error code in a human readable format
   const char* toString( CVErrorCodes code ) {
      switch( code )
         {
         case cvSuccess:
            return "Operation completed successfully";
            break;
         case cvBusError:
            return "VME bus error during the cycle";
            break;
         case cvCommError:
            return "Communication error";
            break;
         case cvGenericError:
            return "Unspecified error";
            break;
         case cvInvalidParam:
            return "Invalid parameter";
            break;
         case cvTimeoutError:
            return "Timeout error";
            break;
         default:
            return "Unknown error";
            break;
         }
   }

} // private namespace

#ifdef Q_OS_WIN

/// Macro checking the return value of a CAEN Digitizer function
#define CHECK( CMD ) {                                                  \
      CVErrorCodes code = CMD;                                          \
      if( code != cvSuccess ) {                                         \
         REPORT_ERROR( qApp->translate( "VmeBus_CHECK",                 \
                                        "Failed executing \"%1\", "     \
                                        "Return value: %2" )            \
                       .arg( #CMD )                                     \
                       .arg( toString( code ) ) );                      \
         return false;                                                  \
      }                                                                 \
   } while( 0 )

#else

/// Macro checking the return value of a CAEN Digitizer function
#define CHECK( CMD ) {                                                  \
      sigprocmask( SIG_BLOCK, &m_blockedSignals, NULL );                \
      CVErrorCodes code = CMD;                                          \
      sigprocmask( SIG_UNBLOCK, &m_blockedSignals, NULL );              \
      if( code != cvSuccess ) {                                         \
         REPORT_ERROR( qApp->translate( "VmeBus_CHECK",                 \
                                        "Failed executing \"%1\", "     \
                                        "Return value: %2" )            \
                       .arg( #CMD )                                     \
                       .arg( toString( code ) ) );                      \
         return false;                                                  \
      }                                                                 \
   } while( 0 )

#endif // Q_OS_WIN
#endif // CAEN libs

namespace {

   /// Helper function translating the board type to a printable value
   const char* toString( caen::VmeBus::BoardType type ) {
      switch( type )
         {
         case caen::VmeBus::BOARD_V1718:
            return "BOARD_V1718";
            break;
         case caen::VmeBus::BOARD_V2718:
            return "BOARD_V2718";
            break;
         case caen::VmeBus::BOARD_A2818:
            return "BOARD_A2818";
            break;
         case caen::VmeBus::BOARD_A2719:
            return "BOARD_A2719";
            break;
         default:
            return "UNKNOWN";
         }
   }

} // private namespace

namespace caen {

   VmeBus::VmeBus()
      : m_handle( -1 ), m_logger( "caen::VME" ) {

      // Initialize the blocked signal list:
#ifndef Q_OS_WIN
      sigfillset( &m_blockedSignals );
      sigaddset( &m_blockedSignals, SIGINT );
      sigaddset( &m_blockedSignals, SIGTERM );
#endif // Q_OS_WIN
   }

   VmeBus::~VmeBus() {

      close();
   }

   bool VmeBus::open( BoardType type, short linkNumber, short boardNumber ) {

      REPORT_VERBOSE( tr( "Open called with type = %1, linkNumber = %2, "
                          "boardNumber = %3" ).arg( toString( type ) )
                      .arg( linkNumber ).arg( boardNumber ) );

#ifdef HAVE_CAEN_VME_LIBS
      CHECK( CAENVME_Init( convert( type ), linkNumber, boardNumber,
                           &m_handle ) );
#endif // CAEN libs

      m_logger << msg::DEBUG << tr( "Opened connection to VME device" )
               << msg::endmsg;

      // Return gracefully:
      return true;
   }

   bool VmeBus::close() {

      // Check if the device is even open:
      if( m_handle == -1 ) {
         m_logger << msg::DEBUG
                  << tr( "No need to close the device, it's not open" )
                  << msg::endmsg;
         return true;
      }

      // Close the connection:
#ifdef HAVE_CAEN_VME_LIBS
      CHECK( CAENVME_End( m_handle ) );
#endif // CAEN libs

      // Reset the device handle:
      m_handle = -1;

      // Let the user know what we did:
      m_logger << msg::DEBUG
               << tr( "Closed connection to VME device" )
               << msg::endmsg;

      // Return gracefully:
      return true;
   }

   int32_t VmeBus::handle() const {

      // Return the handle value:
      return m_handle;
   }

} // namespace caen