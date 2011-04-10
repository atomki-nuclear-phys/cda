// $Id$

// Qt include(s):
#include <QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Event.h"
#   include "cdacore/event/EventServer.h"
#else
#   include "event/Event.h"
#   include "event/EventServer.h"
#endif

// Local include(s):
#include "GlomemWriter.h"
#include "Crate.h"

namespace glomem {

   GlomemWriter::GlomemWriter( ev::EventServer& evserver, Crate& crate,
                               QObject* parent )
      : QThread( parent ), m_evserver( evserver ), m_crate( crate ),
        m_processedEvents( 0 ), m_logger( "glomem::GlomemWriter" ) {

   }

   quint32 GlomemWriter::processedEvents() const {

      return m_processedEvents;
   }

   void GlomemWriter::stopProcessing() {

      // Signal the thread that it should stop, and wait for
      // it to happen:
      quit();
      wait();

      // Print some info of what happened:
      m_logger << msg::INFO
               << tr( "Stopping thread after processing %1 events" ).arg( m_processedEvents )
               << msg::endmsg;

      return;
   }

   void GlomemWriter::run() {

      // Connect the the server's signal(s) to this thread:
      connect( &m_evserver, SIGNAL( eventAvailable() ),
               this, SLOT( displayEvent() ), Qt::DirectConnection );

      // Reset the event counter:
      m_processedEvents = 0;

      // Make sure that any cached events are read out right away:
      displayEvent();

      // Run the thread:
      int retval = 0;
      if( ( retval = exec() ) ) {
         m_logger << msg::FATAL
                  << tr( "Thread exited with return code: %1" ).arg( retval )
                  << msg::endmsg;
      }

      return;
   }

   void GlomemWriter::displayEvent() {

      // Read out all the events that are in the buffer at the moment:
      for( size_t i = 0; i < m_evserver.bufferSize(); ++i ) {

         // Read out one event from the buffer:
         ev::Event event;
         m_evserver >> event;

         // Display this event:
         if( ! m_crate.displayEvent( event ) ) {
            m_logger << msg::FATAL << tr( "There was a problem diplaying an event" )
                     << msg::endmsg;
            exit( 1 );
         }

         // Increment the event counter:
         ++m_processedEvents;

      }

      return;
   }

} // namespace glomem
