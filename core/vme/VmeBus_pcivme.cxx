// $Id$

// System include(s):
#include <iomanip>

// VME library include(s):
#ifdef HAVE_VME_LIB
#include "pcivme_ni.h"
#endif // HAVE_VME_LIB

// CDA include(s):
#include "../common/Sleep.h"

// Local include(s):
#include "VmeBus.h"

namespace vme {

#ifndef HAVE_VME_LIB
   // These are some variables that are used when simulating VME bus access.
   static const unsigned int fifoEmty = 0x7fffffff;
   static const unsigned int eEvent = 0x80000000;
   static int eventCounter = 0;
#endif // HAVE_VME_LIB

   /**
    * Constructor for the class. It just sets some variables to their
    * default state.
    */
   VmeBus::VmeBus( const char* dev_path )
      : m_vmeFD( 0 ), m_path( dev_path ),
#ifdef HAVE_VME_LIB
        m_addressModifier( 0x39 ),
#endif // HAVE_VME_LIB
        m_opened( false ), m_logger( "VmeBus" ) {

      REPORT_VERBOSE( tr( "Creating object" ) );
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
    * Opens the VME bus. In testing mode, it assigns 1 to vmeFD.
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

#ifdef HAVE_VME_LIB
      if( ! VMEopen( dev_path, m_addressModifier,
                     &m_vmeFD ) ) {
         m_opened = true;
      } else {
         REPORT_ERROR( tr( "The PCIVME driver could not be opened" ) );
         return 1;
      }
#else
      m_opened = true;
      m_vmeFD = 1;
#endif // HAVE_VME_LIB

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

#ifdef HAVE_VME_LIB
      if( VMEclose( m_vmeFD ) ) {
         REPORT_ERROR( tr( "Failed to close the PCIVME driver" ) );
         return 1;
      }
#endif // HAVE_VME_LIB

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
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEwrite( m_vmeFD, address, sizeof( char ), 1, &data );
#else
      REPORT_VERBOSE( tr( "8bit write > Address: 0x%1; Data: %2" )
                      .arg( address ).arg( data ) );
#endif // HAVE_VME_LIB

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
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEwrite( m_vmeFD, address, sizeof( int16_t ), 1, &data );
#else
      REPORT_VERBOSE( tr( "16bit write > Address: 0x%1; Data: %2" )
                      .arg( address ).arg( data ) );
#endif // HAVE_VME_LIB

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
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEwrite( m_vmeFD, address, sizeof( int32_t ), 1, &data );
#else
      REPORT_VERBOSE( tr( "32bit write > Address: 0x%1; Data: %2" )
                      .arg( address ).arg( data ) );
#endif // HAVE_VME_LIB

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
   int VmeBus::write( Address_t address, void*
#ifdef HAVE_VME_LIB
                                               data
#endif // HAVE_VME_LIB
                      , unsigned int length ) {

      if( ! isOpen() ) {
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEwrite( m_vmeFD, address, sizeof( char ), length, data );
#else
      REPORT_VERBOSE( tr( "Unknown length write > Address: 0x%1; length: %3" )
                      .arg( address ).arg( length ) );
#endif // HAVE_VME_LIB

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
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEread( m_vmeFD, address, sizeof( char ), 1, data );
#else
      REPORT_VERBOSE( tr( "8-bit read request from address: %1" )
                      .arg( address ) );
      *data = 100;
#endif // HAVE_VME_LIB

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
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEread( m_vmeFD, address, sizeof( int16_t ), 1, data );
#else
      REPORT_VERBOSE( tr( "16-bit read request from address: %1" )
                      .arg( address ) );
      *data = 1000;
#endif // HAVE_VME_LIB

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
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEread( m_vmeFD, address, sizeof( int32_t ), 1, data );
#else
      REPORT_VERBOSE( tr( "32-bit read request from address: %1" )
                      .arg( address ) );

      if( ( eventCounter >= 0 ) && ( eventCounter < 32 ) ) {
         *data = ( ( eventCounter << 20 ) & 0x01f00000 ) |
            ( eventCounter * 100 + 100 );
         ++eventCounter;
      } else if( eventCounter == 33 ) {
         *data = static_cast< int32_t >( fifoEmty );
         eventCounter = 0;
      } else if( eventCounter == 32 ) {
         *data = static_cast< int32_t >( eEvent );
         ++eventCounter;
      }
#endif // HAVE_VME_LIB

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
   int VmeBus::read( Address_t address, void*
#ifdef HAVE_VME_LIB
                                              data
#endif // HAVE_VME_LIB
                     , unsigned int length ) const {

      if( ! isOpen() ) {
         REPORT_ERROR( tr( "VME bus not open yet" ) );
         return 1;
      }

#ifdef HAVE_VME_LIB
      VMEread( m_vmeFD, address, sizeof( char ), length, data );
#else
      REPORT_VERBOSE( tr( "Unknown size read request from Address: %1; length: %2" )
                      .arg( address ).arg( length ) );
#endif // HAVE_VME_LIB

      return 0;
   }

   /**
    * Function that waits for the shortest possible time on the platform.
    * Used in the readout of the VME devices when waiting for new data.
    */
   void VmeBus::wait() {

      common::SleepMin();

      return;
   }

} // namespace vme
