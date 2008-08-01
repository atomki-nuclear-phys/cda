// $Id$

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>

// Local include(s):
#include "EventServer.h"
#include "BinaryStream.h"

namespace ev {

   /**
    * The constructor just sets sensible default values to the member
    * variables but it doesn't start anything.
    */
   EventServer::EventServer( QObject* parent )
      : QThread( parent ), m_mutex(), m_address(),
        m_logger( "ev::EventServer" ) {

   }

   /**
    * This function starts the event reading thread. The thread listens
    * on the address specified to the function. Note that this function
    * automatically stops the thread if it is running at calling time.
    * However it needs to call the QThread::terminate() function for this,
    * which is considered to be a dangerous function to use. So if possible,
    * only call this function once for any given object.
    *
    * @param address The address on which the server should listen
    */
   void EventServer::listen( const Address& address ) {

      // Stop the thread if it is running right now:
      if( isRunning() ) {
         m_logger << msg::WARNING
                  << tr( "Restarting the event reading thread.\n"
                         "This could be dangerous..." )
                  << msg::endmsg;
         quit();
         wait();
      }

      // Start a new thread:
      m_address = address;
      start();

      return;

   }

   /**
    * This function reads one event from the internal buffer. If the buffer
    * is empty, the operator waits until an event becomes available.
    *
    * @param event The event that should be filled with data
    * @returns This same object
    */
   EventServer& EventServer::operator>> ( Event& event ) {

      // Wait for data to become available:
      for( ; ; ) {
         if( ! m_events.size() ) {
            msleep( 10 );
         } else {
            break;
         }
      }

      // Get the "oldest" event and remove it from the buffer:
      event = m_events.front();
      m_mutex.lock();
      m_events.erase( m_events.begin() );
      m_mutex.unlock();

      return *this;

   }

   /**
    * This is the main function of the event reading thread. It creates
    * a QTcpServer and uses it to read events in a never ending loop.
    */
   void EventServer::run() {

      //
      // Start the TCP server:
      //
      QTcpServer server;
      if( server.listen( m_address.getHost(), m_address.getPort() ) ) {
         m_logger << msg::VERBOSE
                  << tr( "Server is listening on host \"%1\" and port %2" )
            .arg( m_address.getHost().toString() ).arg( m_address.getPort() )
                  << msg::endmsg;
      } else {
         m_logger << msg::ERROR
                  << tr( "Server could not be started on host \"%1\" and "
                         "port \"%2\"" )
            .arg( m_address.getHost().toString() ).arg( m_address.getPort() )
                  << msg::endmsg;
         return;
      }

      //
      // Start a never-ending loop:
      //
      for( ; ; ) {

         //
         // Wait indefinitely for an incoming connection:
         //
         if( server.waitForNewConnection( -1 ) ) {
            m_logger << msg::VERBOSE
                     << tr( "Received new incoming connection" )
                     << msg::endmsg;
         } else {
            m_logger << msg::ERROR
                     << tr( "There was a problem while waiting for an "
                            "incoming connection" ) << msg::endmsg;
            return;
         }

         //
         // Get the TCP socket and wait until data becomes available
         // on it. (This function actually reads the data...)
         //
         QTcpSocket* socket = server.nextPendingConnection();
         if( socket->waitForReadyRead( -1 ) ) {
            m_logger << msg::VERBOSE
                     << tr( "Data is ready for readout" )
                     << msg::endmsg;
         } else {
            m_logger << msg::ERROR
                     << tr( "There was a problem while waiting for "
                            "incoming data" )
                     << msg::endmsg;
            return;
         }

         //
         // A little debugging message:
         //
         m_logger << msg::VERBOSE << "Bytes available: "
                  << socket->bytesAvailable() << msg::endmsg;

         //
         // Read the event from the socket:
         //
         Event event;
         BinaryStream stream( socket );
         stream >> event;

         //
         // Add the event to the internal buffer:
         //
         m_mutex.lock();
         m_events.push_back( event );
         m_mutex.unlock();

         delete socket;

      }

      return;

   }

}