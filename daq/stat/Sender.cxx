// $Id$

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "Sender.h"
#include "BinaryStream.h"

namespace cdastat {

   /**
    * @param updateTimeout The time in miliseconds between sending two reports
    *                      to the receivers
    * @param parent The Qt parent of the object
    */
   Sender::Sender( unsigned long updateTimeout, QObject* parent )
      : QThread( parent ), m_mutex(), m_updateTimeout( updateTimeout ),
        m_lastStat(), m_logger( "stat::Sender" ) {

      m_logger << msg::VERBOSE << tr( "Object created" ) << msg::endmsg;

   }

   Sender::~Sender() {

      m_logger << msg::VERBOSE << tr( "Object deleted" ) << msg::endmsg;

   }

   /**
    * @param address Network address to send statistics information to
    */
   void Sender::addReceiver( const Address& address ) {

      m_addresses.push_back( address );
      return;

   }

   /**
    * @param stat The statistics object that should be distributed
    */
   void Sender::update( const Statistics& stat ) {

      m_mutex.lock();
      m_lastStat = stat;
      m_mutex.unlock();

      return;

   }

   /**
    * This is the function executed in a parallel thread. First it connects to all
    * the statistics receivers, then it starts sending out the latest statistics
    * information every m_updateTimeout milisecons.
    */
   void Sender::run() {

      // Sockets where statistics should be sent to:
      std::list< QTcpSocket* > sockets;

      //
      // Make the connection to all the receivers:
      //
      for( std::list< Address >::const_iterator address = m_addresses.begin();
           address != m_addresses.end(); ++address ) {

         // Create a new socket:
         QTcpSocket *socket = new QTcpSocket();

         m_logger << msg::DEBUG << tr( "Connecting to %1:%2" )
            .arg( address->getHost().toString() ).arg( address->getPort() )
                  << msg::endmsg;

         //
         // Try to connect it to the specified receiver:
         //
         socket->connectToHost( address->getHost(), address->getPort(),
                                QIODevice::WriteOnly );
         if( ! socket->waitForConnected( 5000 ) ) {
            printError( *socket );
            delete socket;
            continue;
         } else {
            if( ! socket->open( QIODevice::WriteOnly ) ) {
               printError( *socket );
               delete socket;
               continue;
            }
         }

         //
         // Save the socket for later:
         //
         sockets.push_back( socket );
         m_logger << msg::INFO << tr( "Connected to statistics receiver on %1:%2" )
            .arg( address->getHost().toString() ).arg( address->getPort() )
                  << msg::endmsg;

      }

      //
      // Send out statistics in an endless loop:
      //
      for( ; ; ) {

         //
         // Send the current statistics information to all the sockets:
         //
         for( std::list< QTcpSocket* >::const_iterator socket = sockets.begin();
              socket != sockets.end(); ++socket ) {

            //
            // Check the validity of the socket:
            //
            if( ! ( *socket )->isValid() ) {
               printError( **socket );
               continue;
            }

            //
            // Send the statistics object:
            //
            BinaryStream out( *socket );
            m_mutex.lock();
            out << m_lastStat;
            m_mutex.unlock();
            ( *socket )->flush();

         }

         // Sleep for a little while before sending out the information again:
         msleep( m_updateTimeout );

      }

      return;

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

} // namespace cdastat
