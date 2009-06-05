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

   /**
    * The constructor sets up all the member variables.
    *
    * @param confFileName Optional name of the configuration file that should be served
    * @param parent Qt parent of the server object
    */
   ConfServer::ConfServer( const QString& configFileName, QObject* parent )
      : QTcpServer( parent ), m_configFileName( configFileName ),
        m_loader(), m_logger( "conf::ConfServer" ) {

      //
      // Load all the device plugins:
      //
      if( m_loader.loadAll() ) {
         m_logger << msg::DEBUG << "Successfully loaded all available devices"
                  << msg::endmsg;
      } else {
         m_logger << msg::FATAL << "There was an error loading the devices"
                  << msg::endmsg;
      }

      //
      // Open the internal buffer for reading and writing:
      //
      if( m_buffer.open( QBuffer::ReadWrite ) ) {
         m_logger << msg::VERBOSE << "Opened internal buffer in read/write mode"
                  << msg::endmsg;
      } else {
         m_logger << msg::ERROR << "Internal buffer could not be read in read/write mode!"
                  << msg::endmsg;
      }

      //
      // Read the configuration if a file name was specified:
      //
      if( m_configFileName != "" ) readConfiguration();

      //
      // Set up the handling of the incoming connections:
      //
      connect( this, SIGNAL( newConnection() ),
               this, SLOT( sendConfiguration() ) );

   }

   /**
    * This function puts the server into a listening state, after which it will be
    * able to serve incoming connections.
    *
    * @param address The address on which the incoming connections should be handled
    * @returns <code>true</code> if the server could be started,
    *          <code>false</code> otherwise
    */
   bool ConfServer::listen( const Address& address ) {

      return QTcpServer::listen( address.getHost(), address.getPort() );

   }

   /**
    * This function should be used to specify a new configuration file that should
    * be broadcast. If it differs from the configuration file currently loaded,
    * then the buffer is rewritten with the new configuration information.
    *
    * @param fileName Name of the configuration file to cache
    * @returns <code>true</code> if the configuration could be cached successfully,
    *          <code>false</code> otherwise
    */
   bool ConfServer::setConfigFileName( const QString& fileName ) {

      // If it's the same as the current configuration, don't bother doing anything:
      if( fileName == m_configFileName ) return true;

      m_configFileName = fileName;
      return readConfiguration();

   }

   /**
    * @returns The name of the currently used configuration file
    */
   const QString& ConfServer::getConfigFileName() const {

      return m_configFileName;

   }

   /**
    * This is a very simple slot, that sends the configuration to all the clients that
    * connect to this server. It's pretty much a copy of the "fortune server" Qt code
    * example...
    */
   void ConfServer::sendConfiguration() {

      //
      // Access the socket that just connected:
      //
      QTcpSocket* connection = nextPendingConnection();
      connect( connection, SIGNAL( disconnected() ),
               connection, SLOT( deleteLater() ) );

      //
      // Write the configuration from the buffer into the socket:
      //
      connection->write( m_buffer.buffer() );
      m_logger << msg::VERBOSE << "Bytes to be written: "
               << connection->bytesToWrite() << msg::endmsg;

      //
      // Make sure to actually send the data:
      //
      connection->flush();
      m_logger << msg::DEBUG << "Served configuration to: "
               << connection->peerName() << ":" << connection->peerPort()
               << msg::endmsg;

      //
      // Once the data has been sent, break up the connection:
      //
      connection->disconnectFromHost();

      return;

   }

   /**
    * This function is used internally to detect what is the format of the
    * specified configuration file. Then it calls the corresponding private
    * function to actually read the file. The decision is based on the extension
    * of the configuration file name. If it ends in ".cbin", it's assumed to be
    * in binary format. In all other cases it's assumed to be an XML file.
    *
    * @returns <code>true</code> if the configuration could be cached successfully,
    *          <code>false</code> otherwise
    */
   bool ConfServer::readConfiguration() {

      bool retval;
      if( m_configFileName.endsWith( ".cbin" ) ) {
         retval = readBinaryConfig();
      } else {
         retval = readXMLConfig();
      }

      m_logger << msg::DEBUG << "Size of configuration buffer: "
               << m_buffer.buffer().size() << msg::endmsg;

      return retval;

   }

   /**
    * This function caches the configuration from the currently configured
    * binary file.
    *
    * @returns <code>true</code> if the configuration could be cached successfully,
    *          <code>false</code> otherwise
    */
   bool ConfServer::readBinaryConfig() {

      //
      // Open the file for (binary) reading:
      //
      QFile configFile( m_configFileName );
      if( ! configFile.open( QFile::ReadOnly ) ) {
         m_logger << msg::ERROR << "Couldn't open file: " << m_configFileName
                  << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Opened file: " << m_configFileName << msg::endmsg;
      }

      //
      // Use a temporary crate object to read this configuration:
      //
      Crate crate;
      crate.setLoader( &m_loader );
      if( ! crate.readConfig( &configFile ) ) {
         m_logger << msg::ERROR << "Some error happened while reading the (binary) file: "
                  << m_configFileName << std::endl
                  << "Configuration for the server was not updated!" << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Successfully read the binary configuration"
                  << msg::endmsg;
      }

      //
      // Clear out the internal buffer:
      //
      m_buffer.buffer().clear();

      //
      // Write the configuration into the internal buffer:
      //
      if( ! crate.writeConfig( &m_buffer ) ) {
         m_logger << msg::ERROR << "Some error happened while writing the configuration"
                  << std::endl
                  << "into the internal buffer." << std::endl
                  << "Configuration buffer cleared!" << msg::endmsg;
         m_buffer.buffer().clear();
         return false;
      } else {
         m_logger << msg::DEBUG << "Updated internal buffer from: "
                  << m_configFileName << msg::endmsg;
      }

      return true;

   }

   /**
    * This function caches the configuration from the currently configured
    * XML file.
    *
    * @returns <code>true</code> if the configuration could be cached successfully,
    *          <code>false</code> otherwise
    */
   bool ConfServer::readXMLConfig() {

      //
      // Open the file for (textual) reading:
      //
      QFile configFile( m_configFileName );
      if( ! configFile.open( QFile::ReadOnly | QFile::Text ) ) {
         m_logger << msg::ERROR << "Couldn't open file: " << m_configFileName
                  << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Opened file: " << m_configFileName << msg::endmsg;
      }

      //
      // Parse the file as an XML document:
      //
      QDomDocument doc;
      QString errorMsg;
      int errorLine, errorColumn;
      if( ! doc.setContent( &configFile, false, &errorMsg, &errorLine, &errorColumn ) ) {
         m_logger << msg::ERROR << "Error in parsing \"" << m_configFileName << "\""
                  << std::endl
                  << "  Error message: " << errorMsg << std::endl
                  << "  Error line   : " << errorLine << std::endl
                  << "  Error column : " << errorColumn << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Successfully parsed: " << m_configFileName
                  << msg::endmsg;
      }

      //
      // Decode this XMl document using a temporary crate object:
      //
      Crate crate;
      crate.setLoader( &m_loader );
      if( ! crate.readConfig( doc.documentElement() ) ) {
         m_logger << msg::ERROR << "Failed to read configuration file!" << msg::endmsg;
         return false;
      } else {
         m_logger << msg::VERBOSE << "Successfully read configuration file" << msg::endmsg;
      }

      //
      // Clear out the internal buffer:
      //
      m_buffer.buffer().clear();

      //
      // Write the configuration into the internal buffer:
      //
      if( ! crate.writeConfig( &m_buffer ) ) {
         m_logger << msg::ERROR << "Some error happened while writing the configuration"
                  << std::endl
                  << "into the internal buffer." << std::endl
                  << "Configuration buffer cleared!" << msg::endmsg;
         m_buffer.buffer().clear();
         return false;
      } else {
         m_logger << msg::DEBUG << "Updated internal buffer from: "
                  << m_configFileName << msg::endmsg;
      }

      return true;

   }

} // namespace conf
