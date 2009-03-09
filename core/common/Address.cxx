// $Id$

// Qt include(s):
#include <QtCore/QStringList>

// CDA include(s):
#include "msg/Logger.h"

// Local include(s):
#include "Address.h"

/**
 * This constructor is used when the host name and port number are "encoded" into
 * a single string. The constructor tries to decode this string.
 *
 * The pitfall is, that this class is often used to describe the address of message
 * servers. So for instance when creating the address of the first message server that
 * should be used by the application, these messages have nowhere to go to. For this
 * case there is a little hack in msg::Sender...
 *
 * @param address Encoded host name and port, something like "localhost:49700"
 */
Address::Address( const QString& address )
   : m_host(), m_port( 0 ) {

   //
   // Create a logger object and report what we're doing:
   //
   msg::Logger logger( "Address" );
   logger << msg::VERBOSE << "Constructing address from string: \""
          << address << "\"" << msg::endmsg;

   //
   // Split the address string into a host name and a port number:
   //
   QStringList split = address.split( ":", QString::SkipEmptyParts );
   if( split.size() != 2 ) {
      logger << msg::ERROR << "Couldn't decode address: \""
             << address << "\"" << std::endl
             << "Setting to 127.0.0.1:50000" << msg::endmsg;
      m_host.setAddress( "127.0.0.1" );
      m_port = 50000;
      return;
   }

   //
   // Report what we've done:
   //
   logger << msg::VERBOSE << "Decoded host name: " << split.at( 0 )
          << " port: "  << split.at( 1 ) << msg::endmsg;

   m_host.setAddress( split.at( 0 ) );

   //
   // Try to decode the port number:
   //
   bool ok = false;
   m_port = split.at( 1 ).toUInt( &ok );
   if( ! ok ) {
      logger << msg::ERROR << "Couldn't decode port: " << split.at( 1 ) << std::endl
             << "Setting it to 50000" << msg::endmsg;
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
