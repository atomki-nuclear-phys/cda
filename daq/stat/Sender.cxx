// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "Sender.h"
#include "BinaryStream.h"

namespace stat {

   Sender::Sender()
      : m_logger( "stat::Sender" ) {

      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;

   }

   Sender::~Sender() {

      for( std::list< QTcpSocket* >::const_iterator socket = m_sockets.begin();
           socket != m_sockets.end(); ++socket ) {
         ( *socket )->disconnectFromHost();
         ( *socket )->waitForDisconnected();
         delete ( *socket );
      }

      m_logger << msg::VERBOSE << tr( "Object deleted" ) << msg::endmsg;

   }

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

   bool Sender::send( const Statistics& stat ) const {

      //
      // Loop over all the specified addresses:
      //
      for( std::list< QTcpSocket* >::const_iterator socket = m_sockets.begin();
           socket != m_sockets.end(); ++socket ) {

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

      return true;

   }

   void Sender::printError( const QTcpSocket& socket ) const {

      m_logger << msg::ERROR
               << tr( "Could not connect to statistics receiver on address \"%1\", "
                      "port \"%2\"" ).arg( socket.peerAddress().toString() )
         .arg( socket.peerPort() ) << msg::endmsg;

      return;

   }

} // namespace stat
