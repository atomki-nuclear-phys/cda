// $Id$

// System include(s):
extern "C" {
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
}

// STL include(s):
#include <iostream>

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
#ifndef TESTING
        m_handle( 0 ),
#endif // TESTING
        m_isOpen( false ),
        m_logger( "camac::Crate" ) {

      m_logger << msg::VERBOSE << "Device constructed" << msg::endmsg;

   }

   /**
    * The destructor takes care of closing the crate if the user forgot
    * about it.
    */
   Crate::~Crate() {

      close();
      m_logger << msg::VERBOSE << "Device destructed" << msg::endmsg;

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

#ifndef TESTING
      if( ( int error = cc32_open( m_deviceName, &m_handle ) ) ) {
         m_logger << msg::ERROR << "CAMAC crate could not be opened"
                  << std::endl
                  << "with message: " << strerror( error ) << msg::endmsg;
         return false;
      }
#endif

      m_logger << msg::DEBUG << "CAMAC crate opened on device: "
               << m_devicePath << msg::endmsg;
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

#ifndef TESTING
      if( ( int error = cc32_close( m_handle ) ) ) {
         m_logger << msg::ERROR << "CAMAC crate closing failed"
                  << std::endl
                  << "with message: " << strerror( error ) << msg::endmsg;
         return false;
      }
#endif

      m_logger << msg::DEBUG << "CAMAC crate closed on device: "
               << m_devicePath << msg::endmsg;
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

#ifndef TESTING
      return cc32_read_word( m_handle, N, A, F );
#else
      m_logger << msg::DEBUG << "readWord(...) called with: N = "
               << N << " A = " << A << " F = " << F << msg::endmsg;
      return ( A * 10 );
#endif // TESTING

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

#ifndef TESTING
      return cc32_read_long( m_handle, N, A, F );
#else
      m_logger << msg::DEBUG << "readLong(...) called with: N = "
               << N << " A = " << A << " F = " << F << msg::endmsg;
      return ( A * 100 );
#endif // TESTING

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

#ifndef TESTING
      cc32_write_word( m_handle, N, A, F, data );
#else
      m_logger << msg::DEBUG << "writeWord(...) called with: N = "
               << N << " A = " << A << " F = " << F 
               << " data = " << data << msg::endmsg;
#endif // TESTING

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

#ifndef TESTING
      cc32_write_long( m_handle, N, A, F, data );
#else
      m_logger << msg::DEBUG << "writeLong(...) called with: N = "
               << N << " A = " << A << " F = " << F 
               << " data = " << data << msg::endmsg;
#endif // TESTING

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

#ifndef TESTING
      if( ( int error = cc32_interrupt_enable( m_handle ) ) ) {
         m_logger << msg::ERROR << "There was a problem enabling interrupts"
                  << std::endl
                  << "on device: " << m_devicePath << msg::endmsg;
         return false;
      }
#else
      m_logger << msg::DEBUG << "Interrupts enabled on device: "
               << m_devicePath << msg::endmsg;
#endif // TESTING

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

#ifndef TESTING
      if( ( int error = cc32_interrupt_disable( m_handle ) ) ) {
         m_logger << msg::ERROR << "There was a problem disabling interrupts"
                  << std::endl
                  << "on device: " << m_devicePath << msg::endmsg;
         return false;
      }
#else
      m_logger << msg::DEBUG << "Interrupts disabled on device: "
               << m_devicePath << msg::endmsg;
#endif // TESTING

      return true;

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

#ifndef TESTING
      int timeout, lam;
      if( ( int error = cc32_wait_event( m_handle, &timeout, &lam ) ) ) {
         m_logger << msg::ERROR << "There was a problem waiting for a LAM "
                  << "interrupt." << std::endl
                  << "Cause: " << strerror( error ) << msg::endmsg;
         return false;
      }
#else
      m_logger << msg::VERBOSE << "Waiting for LAM" << msg::endmsg;

      /*
      // Wait for a 100 miliseconds before returning. This is a good way
      // to produce an eco-friendly 10 Hz readout rate in TESTING mode.
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 100000;
      select( 0, NULL, NULL, NULL, &tv );
      */
      sleep( 1 );
#endif // TESTING

      return true;

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
         m_logger << msg::ERROR << "Trying to use the CAMAC crate without "
                  << "opening it first" << std::endl
                  << "Use the Crate::open(...) function!" << msg::endmsg;
         return false;
      }

      return true;

   }

} // namespace camac
