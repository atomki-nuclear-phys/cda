// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_EVENTSERVER_H
#define CDA_CORE_EVENT_EVENTSERVER_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtCore/QTime>

// CDA include(s):
#include "../common/Address.h"
#include "../msg/Logger.h"

// Local include(s):
#include "Event.h"
#include "IncompleteEvent.h"

namespace ev {

   // Make sure the Qt classes are in scope:
   using QT_PREPEND_NAMESPACE( QThread );
   using QT_PREPEND_NAMESPACE( QMutex );
   using QT_PREPEND_NAMESPACE( QTime );

   /**
    *  @short Server for receiving events over the network
    *
    *         This is a pretty fancy class. It can be used to read events
    *         over the network, from another application that uses
    *         ev::Sender to send the events.
    *
    *         The tricky part is, that I wanted to keep the interface of the
    *         server as simple as possible. I wanted to be able to say in
    *         the code something like this:
    *
    *         <code>
    *           ev::EventServer server;<br/>
    *           server.listen( Address( "127.0.0.1", 45000 ) );<br/>
    *           <br/>
    *           ev::Event event;<br/>
    *           server >> event;
    *         </code>
    *
    *         If all the network access would be done in the operator, the
    *         server would have a lot of "dead-time" while the event is
    *         being processed by the application code. So instead I start
    *         a separate thread inside the application that has the only
    *         purpose of reading the events from the network and storing them
    *         in an internal buffer. The reading operator only has to look
    *         at this buffer and return the "oldest" event from it. (If the
    *         buffer is empty, the read operator waits until there is
    *         something in it.) Sounds a bit complicated, but it's actually
    *         pretty simple code...
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class EventServer : public QThread {

      Q_OBJECT

   public:
      /// Constructor
      EventServer( size_t bufferSize = 10000, QObject* parent = 0 );

      /// Start listening on the specified address
      void listen( const Address& address );

      /// Get the number of currently held events
      size_t bufferSize() const;

      /// Read one event from the server
      EventServer& operator>> ( Event& event );

   signals:
      /// Signal emitted when new events are available
      void eventAvailable();

   protected:
      /// Main function of the reading thread
      virtual void run();

   private:
      QMutex m_mutex; ///< Object to serialise event buffer access
      Address m_address; ///< Address on which the server is listening
      std::vector< Event > m_events; ///< Internal event buffer
      const size_t m_bufferSize; ///< Maximal size for the internal buffer
      QTime m_warningTime; ///< Last time a warning was printed
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Server

} // namespace ev

#endif // CDA_CORE_EVENT_EVENTSERVER_H
