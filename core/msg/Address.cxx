// $Id$

// Local include(s):
#include "Address.h"

namespace msg {

   /**
    * @param host String representation of the host name. Can be an IPv4 number as well.
    * @param port Port number of the host application.
    */
   Address::Address( const QString& host, quint16 port )
      : m_host( host ), m_port( port ) {

   }

   /**
    * @param host String representation of the host name. Can be an IPv4 number as well.
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

} // namespace msg
