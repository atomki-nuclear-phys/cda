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
#   include "cdacore/device/Loader.h"
#else
#   include "msg/Logger.h"
#   include "device/Loader.h"
#endif

// Forward declaration(s):
class Address;

namespace conf {

   class ConfServer : public QTcpServer {

      Q_OBJECT

   public:
      /// Constructor
      ConfServer( const QString& configFileName = "", QObject* parent = 0 );

      /// Listen for incoming connections
      bool listen( const Address& address );

      void setConfigFileName( const QString& fileName );
      const QString& getConfigFileName() const;

   protected slots:
      /// Slot sending the configuration information to a client
      void sendConfiguration();

   protected:
      void readConfiguration();
      void readBinaryConfig();
      void readXMLConfig();

   private:
      QString m_configFileName;
      QBuffer m_buffer;

      dev::Loader m_loader;

      mutable msg::Logger m_logger;

   }; // class ConfServer

} // namespace conf

#endif // CDA_DAQ_CONFIG_CONFSERVER_H
