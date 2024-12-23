// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DAQ_STAT_SENDER_H
#define CDA_DAQ_STAT_SENDER_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QMutex>
#include <QThread>

// CDA include(s):
#include "common/Address.h"
#include "msg/Logger.h"

// Local include(s):
#include "../common/Export.h"
#include "Statistics.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QTcpSocket)

namespace cdastat {

// Bring the Qt class(es) into this namespace:
using QT_PREPEND_NAMESPACE(QTcpSocket);

/**
 *  @short Class sending statistics information over the network
 *
 *         This class can be used to send statistics information to
 *         any number of receivers.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDADAQ_EXPORT Sender : public QThread {

   Q_OBJECT

public:
   /// Constructor
   Sender(unsigned long updateTimeout = 2000, QObject* parent = 0);
   /// Destructor
   ~Sender();

   /// Add an address where statistics information should be sent to
   void addReceiver(const Address& address);

   /// Update the statistics information that should be broadcast.
   void update(const Statistics& stat);

protected:
   /// Function running in the parallel thread
   virtual void run();

private:
   /// Print a connection error message
   void printError(const QTcpSocket& socket) const;

   QMutex m_mutex;  ///< Mutex for locking the statistics object to be broadcast
   const unsigned long
       m_updateTimeout;    ///< Interval between statistics broadcasts
   Statistics m_lastStat;  ///< Current object to be broadcast
   std::list<Address>
       m_addresses;  ///< Addresses which are listening for statistics
   mutable msg::Logger m_logger;  ///< Private logger

};  // class Sender

}  // namespace cdastat

#endif  // CDA_DAQ_STAT_SENDER_H
