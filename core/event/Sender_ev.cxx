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
      :  m_logger( "ev::Sender" ) {
      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;

   }

   /**
    * The destructor have to closes the connections
    */
   Sender::~Sender() {
      for( std::list< QTcpSocket* >::const_iterator socket = m_sockets.begin();
           socket != m_sockets.end(); ++socket ) 
	   {
	   	(*socket)->disconnectFromHost();
	 	(*socket)->waitForDisconnected();
		delete (*socket);
	   }

      m_logger << msg::VERBOSE << tr( "Object deleted" ) << msg::endmsg;


   }

   bool Sender::addSocket( const Address& address ) {
	bool status=true;
	QTcpSocket *socket= new QTcpSocket(); //die at vectors die
        socket->connectToHost( address.getHost(), address.getPort(),
                               QIODevice::WriteOnly );
	 if( ! socket->waitForConnected( 5000 ) ) {
            printError( *socket );
            status = false;
         } else
         if( ! socket->open( QIODevice::WriteOnly ) ) {
            printError( *socket );
            status = false;
         }
	if (status)
	{
      		m_sockets.push_front( socket );//copy
	}
      return status;

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

      bool status = true;

      //
      // Loop over all the specified addresses:
      //
      for( std::list< QTcpSocket* >::const_iterator socket = m_sockets.begin();
           socket != m_sockets.end(); ++socket ) {

         //
         // Open a connection to the event server:
         //
//         socket->connectToHost( address->getHost(), address->getPort(),
//                               QIODevice::WriteOnly );

         //
         // Send the event:
         //
         BinaryStream out( *socket );
         out << event;

         //
         // A little debugging message:
         //
         m_logger << msg::VERBOSE << "Bytes to write: " << (*socket)->bytesToWrite()
                  << msg::endmsg;

         //
         // Close the connection:
         //
  //      socket.disconnectFromHost();
  //      socket.waitForDisconnected();

      }

      return status;

   }

   /**
    * This function is used internally to signal when a network address
    * can't be reached.
    *
    * @param address The address that couldn't be reached
    */
   void Sender::printError( const QTcpSocket& socket ) const {

      m_logger << msg::ERROR
               << tr( "An event could not be sent to address \"%1\", "
                      "port \"%2\"" ).arg( socket.peerAddress().toString() 
         .arg( socket.peerPort() ))<< msg::endmsg;

      return;

   }

} // namespace ev
