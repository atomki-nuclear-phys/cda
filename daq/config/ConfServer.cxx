
// Qt include(s):
#include <QFile>
#include <QDomDocument>
#include <QTcpSocket>

// Local include(s):
#include "ConfServer.h"
#include "Crate.h"

// CDA include(s):
#include "common/Address.h"
#include "common/errorcheck.h"
#include "device/Loader.h"

namespace conf {

   /**
    * The constructor sets up all the member variables.
    *
    * @param confFileName Optional name of the configuration file that should
    *        be served
    * @param parent Qt parent of the server object
    */
   ConfServer::ConfServer( const QString& configFileName, QObject* parent )
      : QTcpServer( parent ), m_configFileName( configFileName ),
        m_logger( "conf::ConfServer" ) {

      //
      // Load all the device plugins:
      //
      if( dev::Loader::instance()->loadAll() ) {
         m_logger << msg::DEBUG
                  << tr( "Successfully loaded all available devices" )
                  << msg::endmsg;
      } else {
         REPORT_FATAL( "There was an error loading the devices" );
      }

      //
      // Open the internal buffer for reading and writing:
      //
      if( m_buffer.open( QBuffer::ReadWrite ) ) {
         REPORT_VERBOSE( tr( "Opened internal buffer in read/write mode" ) );
      } else {
         REPORT_ERROR( tr( "Internal buffer could not be read in read/write "
                           "mode!" ) );
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
    * This function puts the server into a listening state, after which it will
    * be able to serve incoming connections.
    *
    * @param address The address on which the incoming connections should be
    *                handled
    * @returns <code>true</code> if the server could be started,
    *          <code>false</code> otherwise
    */
   bool ConfServer::listen( const Address& address ) {

      return QTcpServer::listen( address.getHost(), address.getPort() );
   }

   /**
    * This function should be used to specify a new configuration file that
    * should be broadcast. If it differs from the configuration file currently
    * loaded, then the buffer is rewritten with the new configuration
    * information.
    *
    * @param fileName Name of the configuration file to cache
    * @returns <code>true</code> if the configuration could be cached
    *          successfully, <code>false</code> otherwise
    */
   bool ConfServer::setConfigFileName( const QString& fileName ) {

      // If it's the same as the current configuration, don't bother doing
      // anything:
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
    * This is a very simple slot, that sends the configuration to all the
    * clients that connect to this server. It's pretty much a copy of the
    * "fortune server" Qt code example...
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
      REPORT_VERBOSE( tr( "Bytes to be written: %1" )
                      .arg( connection->bytesToWrite() ) );

      //
      // Make sure to actually send the data:
      //
      connection->flush();
      m_logger << msg::DEBUG
               << tr( "Served configuration to: %1:%2" )
               .arg( connection->peerName() ).arg( connection->peerPort() )
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
    * @returns <code>true</code> if the configuration could be cached
    *          successfully, <code>false</code> otherwise
    */
   bool ConfServer::readConfiguration() {

      if( m_configFileName.endsWith( ".cbin" ) ) {
         CHECK( readBinaryConfig() );
      } else {
         CHECK( readXMLConfig() );
      }

      m_logger << msg::DEBUG
               << tr( "Size of configuration buffer: %1" )
               .arg( m_buffer.buffer().size() )
               << msg::endmsg;

      return true;
   }

   /**
    * This function caches the configuration from the currently configured
    * binary file.
    *
    * @returns <code>true</code> if the configuration could be cached
    *          successfully, <code>false</code> otherwise
    */
   bool ConfServer::readBinaryConfig() {

      //
      // Open the file for (binary) reading:
      //
      QFile configFile( m_configFileName );
      CHECK( configFile.open( QFile::ReadOnly ) );

      //
      // Use a temporary crate object to read this configuration:
      //
      Crate crate;
      crate.setLoader( dev::Loader::instance() );
      if( ! crate.readConfig( configFile ) ) {
         REPORT_ERROR( tr( "Some error happened while reading the (binary)"
                           "file: %1\n"
                           "Configuration for the server was not updated!" )
                       .arg( m_configFileName ) );
         return false;
      } else {
         REPORT_VERBOSE( tr( "Successfully read the binary configuration" ) );
      }

      //
      // Clear out the internal buffer:
      //
      m_buffer.buffer().clear();

      //
      // Write the configuration into the internal buffer:
      //
      if( ! crate.writeConfig( m_buffer ) ) {
         REPORT_ERROR( tr( "Some error happened while writing the "
                           "configuration\n"
                           "into the internal buffer.\n"
                           "Configuration buffer cleared!" ) );
         m_buffer.buffer().clear();
         return false;
      } else {
         m_logger << msg::DEBUG
                  << tr( "Updated internal buffer from: %1" )
                  .arg( m_configFileName )
                  << msg::endmsg;
      }

      return true;
   }

   /**
    * This function caches the configuration from the currently configured
    * XML file.
    *
    * @returns <code>true</code> if the configuration could be cached
    *          successfully, <code>false</code> otherwise
    */
   bool ConfServer::readXMLConfig() {

      //
      // Open the file for (textual) reading:
      //
      QFile configFile( m_configFileName );
      CHECK( configFile.open( QFile::ReadOnly | QFile::Text ) );

      //
      // Parse the file as an XML document:
      //
      QDomDocument doc;
      QString errorMsg;
      int errorLine, errorColumn;
      if( ! doc.setContent( &configFile, false, &errorMsg, &errorLine,
                            &errorColumn ) ) {
         REPORT_ERROR( tr( "Error in parsing \"%1\"\n"
                           "  Error message: %2\n"
                           "  Error line   : %3\n"
                           "  Error column : %4" )
                       .arg( m_configFileName ).arg( errorMsg )
                       .arg( errorLine ).arg( errorColumn ) );
         return false;
      } else {
         REPORT_VERBOSE( tr( "Successfully parsed: %1" )
                         .arg( m_configFileName ) );
      }

      //
      // Decode this XMl document using a temporary crate object:
      //
      Crate crate;
      crate.setLoader( dev::Loader::instance() );
      CHECK( crate.readConfig( doc.documentElement() ) );

      //
      // Clear out the internal buffer:
      //
      m_buffer.buffer().clear();

      //
      // Write the configuration into the internal buffer:
      //
      if( ! crate.writeConfig( m_buffer ) ) {
         REPORT_ERROR( tr( "Some error happened while writing the "
                           "configuration\n"
                           "into the internal buffer.\n"
                           "Configuration buffer cleared!" ) );
         m_buffer.buffer().clear();
         return false;
      } else {
         m_logger << msg::DEBUG
                  << tr( "Updated internal buffer from: %1" )
                  .arg( m_configFileName )
                  << msg::endmsg;
      }

      return true;
   }

} // namespace conf
