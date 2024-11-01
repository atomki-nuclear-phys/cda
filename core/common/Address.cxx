/**
 *   @file core/common/Address.cxx
 *  @short File implementing the Address class
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */

// Qt include(s):
#include <QStringList>

// CDA include(s):
#include "QStringCompat.h"
#include "../msg/Logger.h"

// Local include(s):
#include "Address.h"

/**
 * This constructor is used when the host name and port number are "encoded"
 * into a single string. The constructor tries to decode this string.
 *
 * The pitfall is, that this class is often used to describe the address of
 * message servers. So for instance when creating the address of the first
 * message server that should be used by the application, these messages have
 * nowhere to go to. For this case there is a little hack in msg::Sender...
 *
 * @param address Encoded host name and port, something like "localhost:49700"
 */
Address::Address( const QString& address )
   : m_host(), m_port( 0 ) {

   //
   // Create a logger object and report what we're doing:
   //
   msg::Logger m_logger( "Address" );
   REPORT_VERBOSE( tr( "Constructing address from string: \"%1\"" )
                   .arg( address ) );

   //
   // Split the address string into a host name and a port number:
   //
   QStringList split = address.split( ":", Qt::SkipEmptyParts );
   if( split.size() != 2 ) {
      REPORT_ERROR( tr( "Couldn't decode address: \"%1\"\n"
                        "Setting to 127.0.0.1:50000" ).arg( address ) );
      m_host.setAddress( "127.0.0.1" );
      m_port = 50000;
      return;
   }

   //
   // Report what we've done:
   //
   REPORT_VERBOSE( tr( "Decoded host name: %1, port: %2" )
                   .arg( split.at( 0 ) ).arg( split.at( 1 ) ) );

   m_host.setAddress( split.at( 0 ) );

   //
   // Try to decode the port number:
   //
   bool ok = false;
   m_port = static_cast< quint16 >( split.at( 1 ).toUInt( &ok ) );
   if( ! ok ) {
      REPORT_ERROR( tr( "Couldn't decode port: %1\n"
                        "Setting it to 50000" ).arg( split.at( 1 ) ) );
      m_port = 50000;
   }

}

/**
 * @param host String representation of the host name. Can be an IPv4
 *             number as well.
 * @param port Port number of the host application.
 */
Address::Address( const QString& host, quint16 port )
   : m_host( host ), m_port( port ) {

}

/**
 * @param host String representation of the host name. Can be an IPv4
 *             number as well.
 */
void Address::setHost( const QHostAddress& host ) {

   m_host = host;
   return;
}

/**
 * @param port Port number of the host application.
 */
void Address::setPort( quint16 port ) {

   m_port = port;
   return;
}

/**
 * @returns The address of the message server.
 */
const QHostAddress& Address::getHost() const {

   return m_host;
}

/**
 * @returns The port of the message server application.
 */
quint16 Address::getPort() const {

   return m_port;
}

/**
 * This function can be used to decide if a given string can be decoded
 * into an Address object or not. The check is not too complicated, it
 * just tests if the string is something like: "blabla:<unsigned int>"
 *
 * @returns <code>true</code> if the decoding seems possible,
 *          <code>false</code> otherwise
 */
bool Address::isAddress( const QString& address ) {

   // Split the string into an address and a port number:
   QStringList split = address.split( ":", Qt::SkipEmptyParts );
   if( split.size() != 2 ) {
      return false;
   }

   // Check if the second part of the string is indeed a number:
   bool ok = false;
   split.at( 1 ).toUInt( &ok );
   if( ! ok ) {
      return false;
   }

   return true;
}
