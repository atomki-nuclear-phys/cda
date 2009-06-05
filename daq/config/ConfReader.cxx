// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "ConfReader.h"

namespace conf {

   /**
    * The constructor just needs to set up all the member variables.
    */
   ConfReader::ConfReader()
      : m_logger( "conf::ConfReader" ) {

      //
      // Open the internal buffer for reading and writing:
      //
      if( m_buffer.open( QBuffer::ReadWrite ) ) {
         m_logger << msg::VERBOSE << "Opened internal buffer in read/write mode"
                  << msg::endmsg;
      } else {
         m_logger << msg::ERROR << "Internal buffer could not be read in read/write mode!"
                  << msg::endmsg;
      }

   }

   /**
    * This is the main function of the class. It connects to a host and reads the
    * configuration data in a "blocking" way, which means that it doesn't need a
    * Qt event loop to function.
    *
    * It's pretty much a rip off of the "blocking fortune client" example of the
    * Qt documentation...
    *
    * @returns <code>true</code> if the data was retrieved successfully,
    *          <code>false</code> otherwise
    */
   bool ConfReader::readFrom( const Address& address ) {

      //
      // Create the socket used for the data retrieval:
      //
      QTcpSocket socket;
      m_logger << msg::DEBUG << "Connecting to "
               << address.getHost().toString() << ":"
               << address.getPort() << msg::endmsg;

      //
      // Connect to the specified host:
      //
      socket.connectToHost( address.getHost(), address.getPort(),
                            QIODevice::ReadOnly );
      if( ! socket.waitForConnected( 10000 ) ) {
         m_logger << msg::ERROR << "Couldn't connect to configuration server on: "
                  << address.getHost().toString() << ":"
                  << address.getPort() << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Connected to configuration server on: "
                  << address.getHost().toString() << ":"
                  << address.getPort() << msg::endmsg;
      }

      //
      // Open the socket for reading:
      //
      if( ! socket.open( QIODevice::ReadOnly ) ) {
         m_logger << msg::ERROR << "Couldn't open socket for reading!" << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Socket opened for reading" << msg::endmsg;
      }

      //
      // Wait until all the data is downloaded:
      //
      if( ! socket.waitForReadyRead( 10000 ) ) {
         m_logger << msg::ERROR << "Coulnd't download configuration data in time!"
                  << msg::endmsg;
         return false;
      }

      m_logger << msg::VERBOSE << "Bytes read: " << socket.bytesAvailable()
               << msg::endmsg;

      //
      // Store the received data in the internal buffer:
      //
      m_buffer.buffer() = socket.readAll();

      return true;

   }

   /**
    * This function provides access to the buffer holding the configuration data.
    * It can be used with something like:
    *
    *  <code>
    *   myCrateObject->readConf( confReader.buffer() );
    *  </code>
    *
    * @returns The cached configuration data
    */
   QIODevice* ConfReader::buffer() {

      // Jump to the beginning of the buffer:
      m_buffer.reset();
      return &m_buffer;

   }

} // namespace conf
