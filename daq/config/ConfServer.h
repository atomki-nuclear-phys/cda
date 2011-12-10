// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_CONFIG_CONFSERVER_H
#define CDA_DAQ_CONFIG_CONFSERVER_H

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QBuffer>
#include <QtNetwork/QTcpServer>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
class Address;

/**
 *  @short Namespace for the configuration distribution classes
 *
 *         This namespace holds the classes which are used to distribute
 *         the configuration over the network.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace conf {

   // Bring the Qt classes into the current scope:
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QBuffer );
   using QT_PREPEND_NAMESPACE( QTcpServer );

   /**
    *  @short Class serving the configuration data to its clients
    *
    *         This class is finally implementing a "real" server. It waits
    *         for incoming connections, and when a process connects to it,
    *         it sends that process the CAMAC crate configuration data.
    *
    *         It can read the configuration data from either an XML or a
    *         binary configuration file. It converts the configuration data
    *         to binary format internally, and stores it in a buffer. Then
    *         it just has to send the contents of this buffer to all the
    *         connecting clients.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class ConfServer : public QTcpServer {

      Q_OBJECT

   public:
      /// Constructor
      ConfServer( const QString& configFileName = "", QObject* parent = 0 );

      /// Listen for incoming connections
      bool listen( const Address& address );

      /// Read the configuration from the specified file
      bool setConfigFileName( const QString& fileName );
      /// Return the name of the currently used configuration file
      const QString& getConfigFileName() const;

   protected slots:
      /// Slot sending the configuration information to a client
      void sendConfiguration();

   protected:
      /// Read the configuration from the specified file
      bool readConfiguration();
      /// Read the configuration from a binary file
      bool readBinaryConfig();
      /// Read the configuration from an XML file
      bool readXMLConfig();

   private:
      QString m_configFileName; ///< Name of the configuration file
      QBuffer m_buffer; ///< Internal buffer storing the binary configuration

      mutable msg::Logger m_logger; ///< Private logger object

   }; // class ConfServer

} // namespace conf

#endif // CDA_DAQ_CONFIG_CONFSERVER_H
