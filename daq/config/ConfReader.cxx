
// Qt include(s):
#include <QTcpSocket>

// Local include(s):
#include "ConfReader.h"

namespace conf {

/**
 * The constructor just needs to set up all the member variables.
 */
ConfReader::ConfReader() : m_logger("conf::ConfReader") {

   //
   // Open the internal buffer for reading and writing:
   //
   if (m_buffer.open(QBuffer::ReadWrite)) {
      REPORT_VERBOSE(tr("Opened internal buffer in read/write mode"));
   } else {
      REPORT_ERROR(
          tr("Internal buffer could not be read in read/write "
             "mode!"));
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
bool ConfReader::readFrom(const Address& address) {

   //
   // Create the socket used for the data retrieval:
   //
   QTcpSocket socket;
   m_logger << msg::DEBUG
            << tr("Connecting to %1:%2")
                   .arg(address.getHost().toString())
                   .arg(address.getPort())
            << msg::endmsg;

   //
   // Connect to the specified host:
   //
   socket.connectToHost(address.getHost(), address.getPort(),
                        QIODevice::ReadOnly);
   if (!socket.waitForConnected(10000)) {
      REPORT_ERROR(tr("Couldn't connect to configuration server on: %1:%2")
                       .arg(address.getHost().toString())
                       .arg(address.getPort()));
      return false;
   } else {
      REPORT_VERBOSE(tr("Connected to configuration server on: %1:%2")
                         .arg(address.getHost().toString())
                         .arg(address.getPort()));
   }

   //
   // Open the socket for reading:
   //
   if (!socket.open(QIODevice::ReadOnly)) {
      REPORT_ERROR(tr("Couldn't open socket for reading!"));
      return false;
   } else {
      REPORT_VERBOSE(tr("Socket opened for reading"));
   }

   //
   // Wait until all the data is downloaded:
   //
   if (!socket.waitForReadyRead(10000)) {
      REPORT_ERROR(tr("Coulnd't download configuration data in time!"));
      return false;
   }

   REPORT_VERBOSE(tr("Bytes read: %1").arg(socket.bytesAvailable()));

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
QIODevice& ConfReader::buffer() {

   // Jump to the beginning of the buffer:
   m_buffer.reset();
   return m_buffer;
}

}  // namespace conf
