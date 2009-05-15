// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "Sender.h"
#include "BinaryStream.h"

namespace ev {

   /**
    * Like many other classes, this class doesn't do much in its constructor
    * either.
    */
   Sender::Sender()
      :  m_sockets(), m_logger( "ev::Sender" ) {

      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;

   }

   /**
    * The destructor have to closes the connections
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
    * This function actually takes care of connecting to the event receiver.
    * There is a little timeout allowed, but in general the event receiver
    * application should already be running when this function is called.
    *
    * @param address Network address to connect to
    * @returns <code>true</code> if the connection was successful,
    *          <code>false</code> otherwise
    */
   bool Sender::addSocket( const Address& address ) {

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
      m_logger << msg::INFO << tr( "Connected to event receiver on %1:%2" )
         .arg( address.getHost().toString() ).arg( address.getPort() )
               << msg::endmsg;

      return true;

   }


   /**
    * This is the main function of this class. It loops over all the
    * network sockets that were specified to it, and tries to send
    * the specified event to all the destinations.
    *
    * @param event The event that has to be sent
    * @returns <code>true</code> if the event could be sent to all
    *          destinations, <code>false</code> otherwise
    */
   bool Sender::send( const Event& event ) const {

      // Result of sending the event to all recepients:
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
         // Send the event:
         //
         BinaryStream out( *socket );
         out << event;
         ( *socket )->flush();

         //
         // A little debugging message:
         //
         m_logger << msg::VERBOSE << "Bytes to write: " << (*socket)->bytesToWrite()
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
               << tr( "Could not connect to event receiver on address \"%1\", "
                      "port \"%2\"" ).arg( socket.peerName() )
         .arg( socket.peerPort() ) << msg::endmsg;

      return;

   }

} // namespace ev
