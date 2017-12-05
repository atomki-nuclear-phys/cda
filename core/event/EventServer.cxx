
// Qt include(s):
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>

// Local include(s):
#include "EventServer.h"
#include "BinaryStream.h"

namespace ev {

   /**
    * The constructor just sets sensible default values to the member
    * variables but it doesn't start anything.
    */
   EventServer::EventServer( size_t bufferSize, QObject* parent )
      : QThread( parent ), m_mutex(), m_address(), m_events(),
        m_bufferSize( bufferSize ), m_warningTime( QTime::currentTime() ),
        m_logger( "ev::EventServer" ) {

      // Reserve the entire buffer right away. Most of the data is
      // kept on the heap in ev::Event, but this could still improve
      // performance by a bit.
      m_events.reserve( m_bufferSize );
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
      m_logger << msg::INFO << tr( "Listening for incoming events on %1:%2" )
         .arg( m_address.getHost().toString() ).arg( m_address.getPort() )
               << msg::endmsg;

      start();

      return;
   }

   /**
    * @returns The content of the internal event buffer
    */
   size_t EventServer::bufferSize() const {

      return m_events.size();
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
         REPORT_VERBOSE( tr( "Server is listening on host \"%1\" and port %2" )
                         .arg( m_address.getHost().toString() )
                         .arg( m_address.getPort() ) );
      } else {
         REPORT_ERROR( tr( "Server could not be started on host \"%1\" and "
                           "port \"%2\"" )
                       .arg( m_address.getHost().toString() )
                       .arg( m_address.getPort() ) );
         return;
      }

      //
      // In order to handle broken connections (let's say because the event
      // sender application is stopped), the connection handling is put
      // into a loop:
      //
      for( ; ; ) {

         //
         // Wait indefinitely for an incoming connection:
         //
         if( ! server.waitForNewConnection( -1 ) ) {
            REPORT_ERROR( tr( "There was a problem while waiting for an "
                              "incoming connection" ) );
            return;
         }

         //
         // Get the TCP socket and wait until data becomes available
         // on it.
         //
         QTcpSocket* socket = server.nextPendingConnection();
         if( ! socket ) {
            REPORT_ERROR( tr( "There was a problem while accepting an "
                              "incoming connection" ) );
            return;
         }

         //
         // Start a never-ending loop:
         // Use sinals (for ex. SIGINT) to stop it.
         //
         Event event;
         BinaryStream stream( socket );
         for( ; ; ) {

            //
            // Only ask for new data if there's no data on the socket right
            // now. (There might be if the data rate from the readout
            // application is high.)
            //
            if( ( ! socket->bytesAvailable() ) &&
                ( ! socket->waitForReadyRead( -1 ) ) ) {
               m_logger << msg::INFO
                        << tr( "Connection to event sender lost." )
                        << msg::endmsg;
               break;
            }

            // Don't terminate the thread while an event is coming in:
            setTerminationEnabled( false );

            //
            // Read the event from the socket:
            //
            try {
               stream >> event;
            } catch( const IncompleteEvent& ) {
               // Throw away the incomplete event:
               continue;
            }

            //
            // Add the event to the internal buffer:
            //
            if( m_events.size() < m_bufferSize ) {
               m_mutex.lock();
               m_events.push_back( event );
               m_mutex.unlock();
            } else {
               if( m_warningTime.secsTo( QTime::currentTime() ) > 2 ) {
                  m_warningTime = QTime::currentTime();
                  m_logger << msg::WARNING
                           << tr( "Can't store any more events in internal buffer\n"
                                  "Throwing away incoming event!" )
                           << msg::endmsg;
               }
            }

            // The thread can be terminated once more:
            setTerminationEnabled( true );

            // Tell the listeners that new events are available:
            emit eventAvailable();
         }

      }

      return;
   }

}
