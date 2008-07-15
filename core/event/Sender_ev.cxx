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
      : m_addresses(), m_logger( "ev::Sender" ) {

      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;

   }

   /**
    * The destructor doesn't have to do anything.
    */
   Sender::~Sender() {

      m_logger << msg::VERBOSE << tr( "Object deleted" ) << msg::endmsg;

   }

   void Sender::addAddress( const Address& address ) {

      m_addresses.push_back( address );
      return;

   }

   const std::vector< Address >& Sender::getAddresses() const {

      return m_addresses;

   }

   /**
    * This is the main function of this class. It loops over all the
    * network addresses that were specified to it, and tries to send
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
      for( std::vector< Address >::const_iterator address = m_addresses.begin();
           address != m_addresses.end(); ++address ) {

         //
         // Open a connection to the event server:
         //
         QTcpSocket socket;
         socket.connectToHost( address->getHost(), address->getPort(),
                               QIODevice::WriteOnly );
         if( ! socket.waitForConnected( 1000 ) ) {
            printError( *address );
            status = false;
            continue;
         }
         if( ! socket.open( QIODevice::WriteOnly ) ) {
            printError( *address );
            status = false;
            continue;
         }

         //
         // Send the event:
         //
         BinaryStream out( &socket );
         out << event;

         //
         // A little debugging message:
         //
         m_logger << msg::VERBOSE << "Bytes to write: " << socket.bytesToWrite()
                  << msg::endmsg;

         //
         // Close the connection:
         //
         socket.disconnectFromHost();
         socket.waitForDisconnected();

      }

      return status;

   }

   /**
    * This function is used internally to signal when a network address
    * can't be reached.
    *
    * @param address The address that couldn't be reached
    */
   void Sender::printError( const Address& address ) const {

      m_logger << msg::ERROR
               << tr( "An event could not be sent to address \"%1\", "
                      "port \"%2\"" ).arg( address.getHost().toString() )
         .arg( address.getPort() ) << msg::endmsg;

      return;

   }

} // namespace ev
