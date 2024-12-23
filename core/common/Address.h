// Dear emacs, this is -*- c++ -*-
/**
 *   @file core/common/Address.h
 *  @short Header file declaring the Address class
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
#ifndef CDA_CORE_COMMON_ADDRESS_H
#define CDA_CORE_COMMON_ADDRESS_H

// Qt include(s):
#include <QCoreApplication>
#include <QHostAddress>
#include <QString>

// Local include(s):
#include "Export.h"

/**
 *  @short The address of a server
 *
 *         This is a simple helper class that describes the
 *         network address under which a server is
 *         available.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDACORE_EXPORT Address {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(Address)

public:
   /// Constructor with combined hostname:port string
   Address(const QString& address = "127.0.0.1:50000");
   /// Constructor with host name and port
   Address(const QString& host, quint16 port);

   /// Set the host name of the address
   void setHost(const QHostAddress& host);
   /// Set the port number of the address
   void setPort(quint16 port);

   /// Get the host name of the address
   const QHostAddress& getHost() const;
   /// Get the port number of the address
   quint16 getPort() const;

   /// Function checking if the specified string can be decoded into an address
   static bool isAddress(const QString& address);

private:
   QHostAddress m_host;  ///< Host name of the address
   quint16 m_port;       ///< Port number of the address

};  // class Address

#endif  // CDA_CORE_COMMON_ADDRESS_H
