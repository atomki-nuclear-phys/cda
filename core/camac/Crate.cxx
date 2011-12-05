// $Id$

// CDA include(s):
#include "../common/Sleep.h"

// Local include(s):
#include "Crate.h"

namespace camac {

   /**
    * The constructor only sets the member variables to sensible defaults,
    * but it doesn't do anything with the hardware. Before using the created
    * class, one has to always use Crate::open()!
    *
    * @param dev_path Path to the CAMAC device
    */
   Crate::Crate( const char* dev_path )
      : m_devicePath( dev_path ),
#ifdef HAVE_CAMAC_LIB
        m_handle( 0 ),
#endif // HAVE_CAMAC_LIB
        m_isOpen( false ),
        m_logger( "camac::Crate" ) {

      m_logger << msg::VERBOSE << tr( "Device constructed" ) << msg::endmsg;
   }

   /**
    * The destructor takes care of closing the crate if the user forgot
    * about it.
    */
   Crate::~Crate() {

      close();
      m_logger << msg::VERBOSE << tr( "Device destructed" ) << msg::endmsg;
   }

   /**
    * This function should be used before any access to the CAMAC crate
    * is performed. Using the pcicc32 library it establishes the connection
    * to the crate.
    *
    * @param dev_path Optional argument, the path to the camac device
    *                 If not set, the value set in the constructor is used.
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::open( const char* dev_path ) {

      if( dev_path ) {
         m_devicePath = dev_path;
      }
      if( m_isOpen ) {
         close();
      }

#ifdef HAVE_CAMAC_LIB
      int error;
      if( ( error = cc32_open( ( char* ) m_devicePath, &m_handle ) ) ) {
         m_logger << msg::ERROR << tr( "CAMAC crate could not be opened\n"
                                       "with message: %1" ).arg( strerror( error ) )
                  << msg::endmsg;
         return false;
      }
#endif

      m_logger << msg::DEBUG
               << tr( "CAMAC crate opened on device: %1" ).arg( m_devicePath )
               << msg::endmsg;
      m_isOpen = true;

      return true;
   }

   /**
    * Using the pcicc32 library, this function closes the connection to the
    * CAMAC crate.
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::close() {

      if( ! m_isOpen ) {
         return true;
      }

#ifdef HAVE_CAMAC_LIB
      int error;
      if( ( error = cc32_close( m_handle ) ) ) {
         m_logger << msg::ERROR << tr( "CAMAC crate closing failed\n"
                                       "with message: %1" ).arg( strerror( error ) )
                  << msg::endmsg;
         return false;
      }
#endif

      m_logger << msg::DEBUG
               << tr( "CAMAC crate closed on device: %1" ).arg( m_devicePath )
               << msg::endmsg;
      m_isOpen = false;

      return true;
   }

   /**
    * This function can be used to read the register settings of the
    * CAMAC modules, as these have to be accessed in a 16-bit manner usually.
    *
    * @param N Address of the CAMAC module
    * @param A Sub-address of the CAMAC module
    * @param F Function code to access the address with
    * @returns The read-out information
    */
   uint16_t Crate::readWord( unsigned int N, unsigned int A, unsigned int F ) {

      if( ! checkOpen() ) return 0;

#ifdef HAVE_CAMAC_LIB
      return cc32_read_word( m_handle, N, A, F );
#else
      m_logger << msg::DEBUG
               << tr( "readWord(...) called with: N = %1, A = %2, "
                      "F = %3" ).arg( N ).arg( A ).arg( F )
               << msg::endmsg;
      return ( A * 10 );
#endif // HAVE_CAMAC_LIB
   }

   /**
    * This function is mostly useful for reading out the data collected by
    * the CAMAC modules.
    *
    * @param N Address of the CAMAC module
    * @param A Sub-address of the CAMAC module
    * @param F Function code to access the address with
    * @returns The read-out information
    */
   uint32_t Crate::readLong( unsigned int N, unsigned int A, unsigned int F ) {

      if( ! checkOpen() ) return 0;

#ifdef HAVE_CAMAC_LIB
      return ( cc32_read_long( m_handle, N, A, F ) & 0xffffff );
#else
      m_logger << msg::DEBUG
               << tr( "readLong(...) called with: N = %1, A = %2, "
                      "F = %3" ).arg( N ).arg( A ).arg( F )
               << msg::endmsg;
      return ( A * 100 );
#endif // HAVE_CAMAC_LIB
   }

   /**
    * This function can be used to get additional information from the
    * CAMAC bus on readout.
    *
    * @param N Address of the CAMAC module
    * @param A Sub-address of the CAMAC module
    * @param F Function code to access the address with
    * @param Q CAMAC response line
    * @param X CAMAC "command accepted" line
    * @returns The read-out information
    */
   uint32_t Crate::readLong( unsigned int N, unsigned int A, unsigned int F,
                             bool& Q, bool& X ) {

      if( ! checkOpen() ) return 0;

#ifdef HAVE_CAMAC_LIB
      uint32_t data = cc32_read_long( m_handle, N, A, F );
      Q = ( data & 0x80000000 ) ? true : false;
      X = ( data & 0x40000000 ) ? true : false;
      return ( data & 0xffffff );
#else
      m_logger << msg::DEBUG
               << tr( "readLong(...) called with: N = %1, A = %2, "
                      "F = %3, Q, X" ).arg( N ).arg( A ).arg( F )
               << msg::endmsg;
      Q = false;
      X = false;
      return ( A * 100 );
#endif // HAVE_CAMAC_LIB
   }

   /**
    * This function can be used to set the configuration registers on the
    * CAMAC modules, as those usually have to be accessed in a 16-bit
    * mode.
    *
    * @param N Address of the CAMAC module
    * @param A Sub-address of the CAMAC module
    * @param F Function code to access the address with
    * @param data The information to write
    */
   void Crate::writeWord( unsigned int N, unsigned int A, unsigned int F,
                          uint16_t data ) {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_word( m_handle, N, A, F, data );
#else
      m_logger << msg::DEBUG
               << tr( "writeWord(...) called with: N = %1, A = %2, F = %3, "
                      "data = %4" ).arg( N ).arg( A ).arg( F ).arg( data )
               << msg::endmsg;
#endif // HAVE_CAMAC_LIB

      return;
   }

   /**
    * I'm not exactly sure when this function is useful, but it is possible
    * to write 32-bits of information with it to a CAMAC module.
    *
    * @param N Address of the CAMAC module
    * @param A Sub-address of the CAMAC module
    * @param F Function code to access the address with
    * @param data The information to write
    */
   void Crate::writeLong( unsigned int N, unsigned int A, unsigned int F,
                          uint32_t data ) {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_long( m_handle, N, A, F, data );
#else
      m_logger << msg::DEBUG
               << tr( "writeLong(...) called with: N = %1, A = %2, F = %3, "
                      "data = %4" ).arg( N ).arg( A ).arg( F ).arg( data )
               << msg::endmsg;
#endif // HAVE_CAMAC_LIB

      return;
   }

   /**
    * Enable the interface card to generate interrupts when the crate
    * controller receives a LAM signal. This function has to be called
    * each time after an interrupt is handled!
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::enableInterrupt() {

      if( ! checkOpen() ) return false;

#ifdef HAVE_CAMAC_LIB
      int error;
      if( ( error = cc32_interrupt_enable( m_handle ) ) ) {
         m_logger << msg::ERROR
                  << tr( "There was a problem enabling interrupts\n"
                         "on device: %1" ).arg( m_devicePath ) << msg::endmsg;
         return false;
      }
#endif // HAVE_CAMAC_LIB

      m_logger << msg::DEBUG
               << tr( "Interrupts enabled on device: %1" ).arg( m_devicePath )
               << msg::endmsg;

      return true;
   }

   /**
    * Disable the interface card to generate interrupts when the crate
    * controller receives a LAM signal.
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::disableInterrupt() {

      if( ! checkOpen() ) return false;

#ifdef HAVE_CAMAC_LIB
      int error;
      if( ( error = cc32_interrupt_disable( m_handle ) ) ) {
         m_logger << msg::ERROR
                  << tr( "There was a problem disabling interrupts\n"
                         "on device: %1" ).arg( m_devicePath ) << msg::endmsg;
         return false;
      }
#endif // HAVE_CAMAC_LIB

      m_logger << msg::DEBUG
               << tr( "Interrupts disabled on device: %1" ).arg( m_devicePath )
               << msg::endmsg;

      return true;
   }

   void Crate::setInhibit() {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_word( m_handle, 27, 0, 16, 0 );
#endif // HAVE_CAMAC_LIB

      m_logger << msg::VERBOSE
               << tr( "Inhibit set on device: %1" ).arg( m_devicePath )
               << msg::endmsg;

      return;
   }

   void Crate::resetInhibit() {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_word( m_handle, 27, 1, 16, 0 );
#endif // HAVE_CAMAC_LIB

      m_logger << msg::VERBOSE
               << tr( "Inhibit reset on device: %1" ).arg( m_devicePath )
               << msg::endmsg;

      return;
   }

   void Crate::initialize() {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_word( m_handle, 0, 1, 16, 0 );
#else
      m_logger << msg::DEBUG
               << tr( "Initialize called on device: %1" ).arg( m_devicePath )
               << msg::endmsg;
#endif // HAVE_CAMAC_LIB

      return;
   }

   void Crate::clear() {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_word( m_handle, 0, 0, 16, 0 );
#else
      m_logger << msg::DEBUG
               << tr( "Clear called on device: %1" ).arg( m_devicePath )
               << msg::endmsg;
#endif // HAVE_CAMAC_LIB

      return;
   }

   void Crate::setLAMMask( uint32_t mask ) {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_long( m_handle, 28, 1, 16, mask );
#else
      m_logger << msg::DEBUG
               << tr( "LAM mask set to: 0x%1" ).arg( QString::number( mask, 16 ) )
               << msg::endmsg;
#endif // HAVE_CAMAC_LIB

      return;
   }

   /**
    * This function should be used in the data acquisition process to wait
    * for a new event. When a new event is registered by the CAMAC devices,
    * one of them sends a LAM signal (if the configuration was correct),
    * the PCI interface card sends an interrupt in turn, and in the end
    * this function returns.
    *
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Crate::waitForLAM() {

      if( ! checkOpen() ) return false;

#ifdef HAVE_CAMAC_LIB
      int timeout, lam;
      int error;
      if( ( error = cc32_wait_event( m_handle, &timeout, &lam ) ) ) {
         m_logger << msg::ERROR << tr( "There was a problem waiting for a LAM "
                                       "interrupt.\nCause: " )
                  << strerror( error ) << msg::endmsg;
         return false;
      }
#else
      m_logger << msg::VERBOSE << tr( "Waiting for LAM" ) << msg::endmsg;
      // Sleep for a hundred miliseconds:
      common::Sleep( 100 );
#endif // HAVE_CAMAC_LIB

      return true;
   }

   void Crate::clearLAM() {

      if( ! checkOpen() ) return;

#ifdef HAVE_CAMAC_LIB
      cc32_write_word( m_handle, 28, 0, 16, 0 );
#else
      m_logger << msg::DEBUG
               << tr( "LAM cleared on device: %1" ).arg( m_devicePath )
               << msg::endmsg;
#endif // HAVE_CAMAC_LIB

      return;
   }

   /**
    * This internal function is called by each function trying to access
    * the CAMAC crate. It checks if the crate is alreay open, and prints
    * an ERROR message if it isn't.
    *
    * @returns <code>true</code> if the crate is open,
    *          <code>false</code> if it is closed
    */
   bool Crate::checkOpen() const {

      if( ! m_isOpen ) {
         m_logger << msg::ERROR
                  << tr( "Trying to use the CAMAC crate without "
                         "opening it first\n"
                         "Use the Crate::open(...) function!" ) << msg::endmsg;
         return false;
      }

      return true;
   }

} // namespace camac
