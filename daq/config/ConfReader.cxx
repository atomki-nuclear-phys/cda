// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "ConfReader.h"

namespace conf {

   ConfReader::ConfReader()
      : m_logger( "conf::ConfReader" ) {

      if( m_buffer.open( QBuffer::ReadWrite ) ) {
         m_logger << msg::VERBOSE << "Opened internal buffer in read/write mode"
                  << msg::endmsg;
      } else {
         m_logger << msg::ERROR << "Internal buffer could not be read in read/write mode!"
                  << msg::endmsg;
      }

   }

   bool ConfReader::readFrom( const Address& address ) {

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

   QIODevice* ConfReader::buffer() {

      // Jump to the beginning of the buffer:
      m_buffer.reset();
      return &m_buffer;

   }

} // namespace conf
