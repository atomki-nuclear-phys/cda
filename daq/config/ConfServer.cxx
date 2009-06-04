// $Id$

// Qt include(s):
#include <QtCore/QFile>
#include <QtXml/QDomDocument>
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "ConfServer.h"
#include "Crate.h"

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#else
#   include "common/Address.h"
#endif

namespace conf {

   ConfServer::ConfServer( const QString& configFileName, QObject* parent )
      : QTcpServer( parent ), m_configFileName( configFileName ),
        m_loader(), m_logger( "conf::ConfServer" ) {

      if( m_loader.loadAll() ) {
         m_logger << msg::DEBUG << "Successfully loaded all available devices"
                  << msg::endmsg;
      } else {
         m_logger << msg::FATAL << "There was an error loading the devices"
                  << msg::endmsg;
      }

      if( m_buffer.open( QBuffer::ReadWrite ) ) {
         m_logger << msg::VERBOSE << "Opened internal buffer in read/write mode"
                  << msg::endmsg;
      } else {
         m_logger << msg::ERROR << "Internal buffer could not be read in read/write mode!"
                  << msg::endmsg;
      }

      if( m_configFileName != "" ) readConfiguration();

      connect( this, SIGNAL( newConnection() ),
               this, SLOT( sendConfiguration() ) );

   }

   bool ConfServer::listen( const Address& address ) {

      return QTcpServer::listen( address.getHost(), address.getPort() );

   }

   void ConfServer::setConfigFileName( const QString& fileName ) {

      if( fileName == m_configFileName ) return;

      m_configFileName = fileName;
      readConfiguration();

      return;

   }

   const QString& ConfServer::getConfigFileName() const {

      return m_configFileName;

   }

   void ConfServer::sendConfiguration() {

      QTcpSocket* connection = nextPendingConnection();
      connect( connection, SIGNAL( disconnected() ),
               connection, SLOT( deleteLater() ) );

      connection->write( m_buffer.buffer() );

      m_logger << msg::VERBOSE << "Bytes to be written: "
               << connection->bytesToWrite() << msg::endmsg;

      connection->flush();

      m_logger << msg::DEBUG << "Served configuration to: "
               << connection->peerName() << ":" << connection->peerPort()
               << msg::endmsg;

      connection->disconnectFromHost();

      return;

   }

   void ConfServer::readConfiguration() {

      if( m_configFileName.endsWith( ".cbin" ) ) {
         readBinaryConfig();
      } else {
         readXMLConfig();
      }

      m_logger << msg::DEBUG << "Size of configuration buffer: "
               << m_buffer.buffer().size() << msg::endmsg;

      return;

   }

   void ConfServer::readBinaryConfig() {

      QFile configFile( m_configFileName );
      if( ! configFile.open( QFile::ReadOnly ) ) {
         m_logger << msg::ERROR << "Couldn't open file: " << m_configFileName << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "Opened file: " << m_configFileName << msg::endmsg;
      }

      Crate crate;
      crate.setLoader( &m_loader );
      if( ! crate.readConfig( &configFile ) ) {
         m_logger << msg::ERROR << "Some error happened while reading the (binary) file: "
                  << m_configFileName << std::endl
                  << "Configuration for the server was not updated!" << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "Successfully read the binary configuration"
                  << msg::endmsg;
      }

      m_buffer.buffer().clear();

      if( ! crate.writeConfig( &m_buffer ) ) {
         m_logger << msg::ERROR << "Some error happened while writing the configuration"
                  << std::endl
                  << "into the internal buffer." << std::endl
                  << "Configuration buffer cleared!" << msg::endmsg;
         m_buffer.buffer().clear();
         return;
      } else {
         m_logger << msg::DEBUG << "Updated internal buffer from: "
                  << m_configFileName << msg::endmsg;
      }

      return;

   }

   void ConfServer::readXMLConfig() {

      QFile configFile( m_configFileName );
      if( ! configFile.open( QFile::ReadOnly | QFile::Text ) ) {
         m_logger << msg::ERROR << "Couldn't open file: " << m_configFileName << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "Opened file: " << m_configFileName << msg::endmsg;
      }

      QDomDocument doc;
      QString errorMsg;
      int errorLine, errorColumn;
      if( ! doc.setContent( &configFile, false, &errorMsg, &errorLine, &errorColumn ) ) {
         m_logger << msg::ERROR << "Error in parsing \"" << m_configFileName << "\""
                  << std::endl
                  << "  Error message: " << errorMsg << std::endl
                  << "  Error line   : " << errorLine << std::endl
                  << "  Error column : " << errorColumn << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "Successfully parsed: " << m_configFileName
                  << msg::endmsg;
      }

      Crate crate;
      crate.setLoader( &m_loader );
      if( ! crate.readConfig( doc.documentElement() ) ) {
         m_logger << msg::ERROR << "Failed to read configuration file!" << msg::endmsg;
         return;
      } else {
         m_logger << msg::VERBOSE << "Successfully read configuration file" << msg::endmsg;
      }

      m_buffer.buffer().clear();

      if( ! crate.writeConfig( &m_buffer ) ) {
         m_logger << msg::ERROR << "Some error happened while writing the configuration"
                  << std::endl
                  << "into the internal buffer." << std::endl
                  << "Configuration buffer cleared!" << msg::endmsg;
         m_buffer.buffer().clear();
         return;
      } else {
         m_logger << msg::DEBUG << "Updated internal buffer from: "
                  << m_configFileName << msg::endmsg;
      }

      return;

   }

} // namespace conf
