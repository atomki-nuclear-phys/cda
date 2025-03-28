//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_APPS_CDA_HBOOK_WRITER_FILEWRITER_H
#define CDA_APPS_CDA_HBOOK_WRITER_FILEWRITER_H

// CDA include(s):
#include "msg/Logger.h"

// Qt include(s):
#include <QThread>

// Forward declaration(s):
namespace ev {
class EventServer;
}

namespace hbook {

// Forward declaration(s):
class Crate;

/**
 *  @short Thread running the file writing
 *
 *         The file writing is now done in a dedicated thread. This makes it
 *         much simpler to have control over what the application is doing,
 *         and to monitor its status.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class FileWriter : public QThread {

   Q_OBJECT

public:
   /// Constructor
   FileWriter(ev::EventServer& evserver, Crate& crate, QObject* parent = 0);

   /// Get the number of events that have been processed so far
   quint32 processedEvents() const;

   /// Stop the execution of the thread
   void stopProcessing();

protected:
   /// Main function of running the thread
   virtual void run();

private slots:
   /// Write one event to the output file
   void writeEvent();

private:
   ev::EventServer& m_evserver;  ///< Pre-configured event server object
   Crate& m_crate;               ///< Pre-configured CAMAC crate object
   quint32 m_processedEvents;    ///< Number of processed events

   msg::Logger m_logger;  ///< Logger object for the class

};  // class FileWriter

}  // namespace hbook

#endif  // CDA_APPS_CDA_HBOOK_WRITER_FILEWRITER_H
