// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_GLOMEM_WRITER_GLOMEMWRITER_H
#define CDA_APPS_CDA_GLOMEM_WRITER_GLOMEMWRITER_H

// Qt include(s):
#include <QtCore/QThread>

// CDA include(s):
#include "msg/Logger.h"

// Forward declaration(s):
namespace ev {
class EventServer;
}

namespace glomem {

// Forward declaration(s):
class Crate;

/**
 *  @short Thread for filling the monitoring histograms
 *
 *         The monitoring histogram writing is now done in a dedicated thread.
 *         This makes it much easier to monitor its progress, and control what
 *         it's doing.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class GlomemWriter : public QThread {

   Q_OBJECT

public:
   /// Constructor
   GlomemWriter(ev::EventServer& evserver, Crate& crate, QObject* parent = 0);

   /// Get the number of events that have been processed so far
   quint32 processedEvents() const;

   /// Stop the event processing
   void stopProcessing();

protected:
   /// Main function of the thread
   virtual void run();

private slots:
   /// Display one event in the histograms
   void displayEvent();

private:
   ev::EventServer& m_evserver;  ///< Pre-configured event server object
   Crate& m_crate;               ///< Pre-configured CAMAC crate object
   quint32 m_processedEvents;    ///< Number of processed events

   mutable msg::Logger m_logger;  ///< Logger object for the class

};  // class GlomemWriter

}  // namespace glomem

#endif  // CDA_APPS_CDA_GLOMEM_WRITER_GLOMEMWRITER_H
