// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#else
#   include "common/Address.h"
#endif

// Local include(s):
#include "Sender.h"
#include "BinaryStream.h"

namespace stat {

   Sender::Sender()
      : m_logger( "stat::Sender" ) {

      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;

   }

   /**
    * The destructor has to close the connection with all the statistics recepients
    * and delete all the objects created by this object.
    */
   Sender::~Sender() {

      for( std::list< QTcpSocket* >::const_iterator socket = m_sockets.begin();
           socket != m_sockets.end(); ++socket ) {
         ( *socket )->disconnectFromHost();
         ( *socket )->waitForDisconnected();
         delete ( *socket );
      }

      m_logger << msg::VERBOSE << tr( "Object deleted" ) << msg::endmsg;

   }

   /**
    * @param address Network address to send statistics information to
    * @returns <code>true</code> if the address could be connected to,
    *          <code>false</code> otherwise
    */
   bool Sender::addReceiver( const Address& address ) {

      // Create a new socket:
      QTcpSocket *socket = new QTcpSocket();

      m_logger << msg::DEBUG << tr( "Connecting to %1:%2" )
         .arg( address.getHost().toString() ).arg( address.getPort() )
               << msg::endmsg;

      //
      // Try to connect it to the specified receiver:
      //
      socket->connectToHost( address.getHost(), address.getPort(),
                             QIODevice::WriteOnly );
      if( ! socket->waitForConnected( 50000 ) ) {
         printError( *socket );
         delete socket;
         return false;
      } else {
         if( ! socket->open( QIODevice::WriteOnly ) ) {
            printError( *socket );
            delete socket;
            return false;
         }
      }

      //
      // Save the socket for later:
      //
      m_sockets.push_back( socket );
      m_logger << msg::INFO << tr( "Connected to statistics receiver on %1:%2" )
         .arg( address.getHost().toString() ).arg( address.getPort() )
               << msg::endmsg;

      return true;

   }

   /**
    * @param stat The statistics object that should be distributed
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
   bool Sender::send( const Statistics& stat ) const {

      // Result of sending the statistics info to all recepients:
      bool result = true;

      //
      // Loop over all the specified addresses:
      //
      for( std::list< QTcpSocket* >::const_iterator socket = m_sockets.begin();
           socket != m_sockets.end(); ++socket ) {

         //
         // Check the validity of the socket:
         //
         if( ! ( *socket )->isValid() ) {
            printError( **socket );
            result = false;
            continue;
         }

         //
         // Send the statistics object:
         //
         BinaryStream out( *socket );
         out << stat;
         ( *socket )->flush();

         //
         // A little debugging message:
         //
         m_logger << msg::VERBOSE << "Bytes to write: " << ( *socket )->bytesToWrite()
                  << msg::endmsg;

      }

      return result;

   }

   /**
    * This function is used internally to signal when a network address
    * can't be reached.
    *
    * @param socket The socket that couldn't be connected
    */
   void Sender::printError( const QTcpSocket& socket ) const {

      m_logger << msg::ERROR
               << tr( "Could not connect to statistics receiver on address \"%1\", "
                      "port \"%2\"" ).arg( socket.peerName() )
         .arg( socket.peerPort() ) << msg::endmsg;

      return;

   }

} // namespace stat
