// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_ADDRESS_H
#define CDA_CORE_COMMON_ADDRESS_H

// Qt include(s):
#include <QtCore/QString>
#include <QtNetwork/QHostAddress>

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "msg::Address needs Qt >= 4.0.0"
#endif

/**
 *  @short The address of a server
 *
 *         This is a simple helper class that describes the
 *         network address under which a server is
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

#endif // CDA_CORE_COMMON_ADDRESS_H