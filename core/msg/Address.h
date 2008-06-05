// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_ADDRESS_H
#define CDA_CORE_MSG_ADDRESS_H

// Qt include(s):
#include <QtCore/QString>
#include <QtNetwork/QHostAddress>

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // MSG namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QHostAddress );

   /**
    *  @short The address of a message server
    *
    *         This is a simple helper class that describes the
    *         network address under which a message server is
    *         available.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Address {

   public:
      /// Constructor with host name and port
      Address( const QString& host = "127.0.0.1",
               quint16 port = 50000 );

      /// Set the host name of the address
      void setHost( const QHostAddress& host );
      /// Set the port number of the address
      void setPort( quint16 port );

      /// Get the host name of the address
      const QHostAddress& getHost() const;
      /// Get the port number of the address
      quint16             getPort() const;

   private:
      QHostAddress m_host; ///< Host name of the address
      quint16      m_port; ///< Port number of the address

   }; // class Address

} // namespace msg

#endif // CDA_CORE_MSG_ADDRESS_H
