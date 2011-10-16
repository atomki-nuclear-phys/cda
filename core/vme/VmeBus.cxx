// $Id$

// System include(s):
#include <iomanip>
extern "C" {
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
}

#ifndef TESTING
#include "pcivme_ni.h"
#endif // TESTING

// Local include(s):
#include "VmeBus.h"

namespace vme {

#ifdef TESTING
   // These are some variables that are used when simulating VME bus access.
   static const unsigned int fifoEmty = 0x7fffffff;
   static const unsigned int eEvent = 0x80000000;
   static int eventCounter = 0;
#endif // TESTING

   /**
    * Constructor for the class. It just sets some variables to their
    * default state.
    */
   VmeBus::VmeBus( const char* dev_path )
      : m_vmeFD( 0 ), m_path( dev_path ), m_addressModifier( 0x39 ),
        m_opened( false ), m_logger( "VmeBus" ) {

      m_logger << msg::VERBOSE << tr( "Creating object" ) << msg::endmsg;;
   }

   /**
    * Destructor for the class. It closes the VME bus if it is still opened.
    */
   VmeBus::~VmeBus() {

      if( isOpen() ) {
         close();
      }
   }

   /**
    * Opens the VME bus. In TESTING mode, it assigns 1 to vmeFD.
    * Failure to open the bus is signalled by a non-zero return value.
    *
    * @returns <code>0</code> if the opening was successful,
    *          something else otherwise
    */
   int VmeBus::open( const char* dev_path ) {

      // Update the device path is one has been specified:
      if( dev_path ) {
         m_path = dev_path;
      }

      // Don't do anything if the bus is already open:
      if( isOpen() ) {
         m_logger << msg::WARNING
                  << tr( "The VME bus is already open" )
                  << msg::endmsg;
         return 0;
      }

#ifndef TESTING
      if( ! VMEopen( m_path.toLatin1().Data(), m_addressModifier,
                     &m_vmeFD ) ) {
         m_opened = true;
      } else {
         m_logger << msg::ERROR
                  << tr( "The PCIVME driver could not be opened" )
                  << msg::endmsg;
         return 1;
      }
#else
      m_opened = true;
      m_vmeFD = 1;
#endif // TESTING

      return 0;
   }

   /**
    * Closes the VME bus. It is automatically called by the default
    * destructor, so normally the user should not care about explicitly
    * calling this function.
    *
    * @returns <code>0</code> if the closing was successful,
    *          something else otherwise
    */
   int VmeBus::close() {

      // Return right away if nothing needs to be done:
      if( ! isOpen() ) {
         return 0;
      }

#ifndef TESTING
      if( VMEclose( m_vmeFD ) ) {
         m_logger << msg::ERROR
                  << "Failed to close the PCIVME driver"
                  << msg::endmsg;
         return 1;
      }
#endif // TESTING

      m_opened = false;

      return 0;
   }

   /**
    * This function gives access to the vmeFD variable that stores the
    * file descriptor with wich the class can access the VME bus. It is
    * absolutely against OO! Actually I don't know if any part of my code
    * relies on it. If you have the time to check it, you should get rid
    * of it. I've put it in for general flexibility. (Youth...)
    *
    * @returns The VME bus's file descriptor.
    */
   int VmeBus::fd() const {

      return m_vmeFD;
   }

   bool VmeBus::isOpen() const {

      return m_opened;
   }

   /**
    * Writes 8 bits of data to the VME bus.
    *
    * @param address The VME bus address to write the data to
    * @param data The data to be written to the VME bus
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::write( Address_t address, char data ) {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEwrite( m_vmeFD, address, sizeof( char ), 1, &data );
#else
      m_logger << msg::VERBOSE
               << tr( "8bit write > Address: 0x%1; Data: %2" )
         .arg( address ).arg( data )
               << msg::endmsg;
#endif // TESTING

      return 0;
   }

   /**
    * Writes 16 bits of data to the VME bus.
    *
    * @param address The VME bus address to write the data to
    * @param data The data to be written to the VME bus
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::write( Address_t address, int16_t data ) {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEwrite( m_vmeFD, address, sizeof( int16_t ), 1, &data );
#else
      m_logger << msg::VERBOSE
               << tr( "16bit write > Address: 0x%1; Data: %2" )
         .arg( address ).arg( data )
               << msg::endmsg;
#endif // TESTING

      return 0;
   }

   /**
    * Writes 32 bits of data to the VME bus.
    *
    * @param address The VME bus address to write the data to
    * @param data The data to be written to the VME bus
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::write( Address_t address, int32_t data ) {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEwrite( m_vmeFD, address, sizeof( int32_t ), 1, &data );
#else
      m_logger << msg::VERBOSE
               << tr( "32bit write > Address: 0x%1; Data: %2" )
         .arg( address ).arg( data )
               << msg::endmsg;
#endif // TESTING

      return 0;
   }

   /**
    * Writes unknown bits of data to the VME bus.
    *
    * @param address The VME bus address to write the data to
    * @param data Pointer to the data to be written to the VME bus
    * @param length The length of the data that should be written to
    *               the VME bus (in bytes)
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::write( Address_t address, void* data,
                      unsigned int length ) {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEwrite( m_vmeFD, address, sizeof( char ), length, data );
#else
      m_logger << msg::VERBOSE
               << tr( "Unknown length write > Address: 0x%1; length: %2" )
         .arg( address ).arg( length )
               << msg::endmsg;
#endif // TESTING

      return 0;
   }

   /**
    * Reads 8 bits of data from the VME bus.
    *
    * @param address The VME bus address to read the data from
    * @param data A pointer to a char* variable where the read data will be written
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::read( Address_t address, char* data ) const {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEread( m_vmeFD, address, sizeof( char ), 1, data );
#else
      *data = 100;
#endif // TESTING

      return 0;
   }

   /**
    * Reads 16 bits of data from the VME bus.
    *
    * @param address The VME bus address to read the data from
    * @param data A pointer to a short* variable where the read data will be written
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::read( Address_t address, int16_t* data ) const {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEread( m_vmeFD, address, sizeof( int16_t ), 1, data );
#else
      *data = 1000;
#endif // TESTING

      return 0;
   }

   /**
    * Reads 32 bits of data from the VME bus.
    *
    * @param address The VME bus address to read the data from
    * @param data A pointer to an int* variable where the read data will be written
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::read( Address_t address, int32_t* data ) const {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEread( m_vmeFD, address, sizeof( int32_t ), 1, data );
#else
      if( ( eventCounter >= 0 ) && ( eventCounter < 32 ) ) {
         *data = ( ( eventCounter << 20 ) & 0x01f00000 ) |
            ( eventCounter * 100 + 100 );
         ++eventCounter;
      } else if( eventCounter == 33 ) {
         *data = fifoEmty;
         eventCounter = 0;
      } else if( eventCounter == 32 ) {
         *data = eEvent;
         ++eventCounter;
      }
#endif // TESTING

      return 0;
   }

   /**
    * Reads unknown bits of data from the VME bus.
    *
    * @param address The VME bus address to read the data from
    * @param data A void* pointer where the read data will be written
    * @param length Length of the data to be read (in bytes)
    * @returns <code>0</code> if the operation was successful,
    *          something else otherwise
    */
   int VmeBus::read( Address_t address, void* data,
                     unsigned int length ) const {

      if( ! isOpen() ) {
         m_logger << msg::ERROR
                  << tr( "VME bus not open yet" )
                  << msg::endmsg;
         return 1;
      }

#ifndef TESTING
      VMEread( m_vmeFD, address, sizeof( char ), length, data );
#endif // TESTING

      return 0;
   }

   /**
    * Function that waits for 'usec' usec-s. It plays a big
    * role in the VME bus readout!
    */
   void VmeBus::wait( int usec ) {

      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = usec;
      ::select( 0, NULL, NULL, NULL, &tv );

      return;
   }

} // namespace vme
