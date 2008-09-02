// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>
#include <QTime>
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
//
// return the diference in ms
// if b is greater we assumes it is mindnight problem, and calculate
// the return value is always positve (or zero)
// QTime is not the best for usage like this
int operator-(const QTime &a,const QTime &b)
{
        static const QTime zero; //Default constructor set it to zero   
        int a_ms=zero.msecsTo(a);
        int b_ms=zero.msecsTo(b);
        if (a_ms>=b_ms) return a_ms-b_ms;
                else return 86400000-b_ms+b_ms;
}

   bool Sender::addSocket( const Address& address ) {
	bool status=true;
	QTcpSocket *socket= new QTcpSocket(); //die at senders die

         m_logger << msg::INFO << tr ("Connecting to %1:%2").arg(address.getHost().toString()).arg(address.getPort())
                  << msg::endmsg;
	QTime begin_time=QTime::currentTime();
	do
	{
		socket->connectToHost( address.getHost(), address.getPort(),
				QIODevice::WriteOnly );
		if( ! socket->waitForConnected( 50000 ) ) {
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
			m_logger << msg::INFO << tr ("Connect to %1:%2 OK").arg(address.getHost().toString()).arg(address.getPort())
				<< msg::endmsg;
		} else
		{
			m_logger << msg::ERROR << tr ("Connect to %1:%2 Failed").arg(address.getHost().toString()).arg(address.getPort());
				
			if (QTime::currentTime()-begin_time>30000)
			{
				m_logger << ", Give up" << msg::endmsg;
				return status;
			}else
			{
				
				m_logger << ", Retrying" << msg::endmsg;
			}
		}
	} while (status);
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
	 (*socket)->flush();
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
                      "port \"%2\"" ).arg( socket.peerAddress().toString()).arg(
          		socket.peerPort() )<< msg::endmsg;

      return;

   }

} // namespace ev
