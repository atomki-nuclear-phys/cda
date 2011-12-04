// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_SENDER_H
#define CDA_DAQ_STAT_SENDER_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QtCore/QThread>
#include <QtCore/QMutex>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "common/Address.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Statistics.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QTcpSocket )

namespace cdastat {

   // Bring the Qt class(es) into this namespace:
   using QT_PREPEND_NAMESPACE( QTcpSocket );

   /**
    *  @short Class sending statistics information over the network
    *
    *         This class can be used to send statistics information to
    *         any number of receivers.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Sender : public QThread {

      Q_OBJECT

   public:
      /// Constructor
      Sender( unsigned long updateTimeout = 2000, QObject* parent = 0 );
      /// Destructor
      ~Sender();

      /// Add an address where statistics information should be sent to
      void addReceiver( const Address& address );

      /// Update the statistics information that should be broadcast.
      void update( const Statistics& stat );

   protected:
      /// Function running in the parallel thread
      virtual void run();

   private:
      /// Print a connection error message
      void printError( const QTcpSocket& socket ) const;

      QMutex m_mutex; ///< Mutex for locking the statistics object to be broadcast
      const unsigned long m_updateTimeout; ///< Interval between statistics broadcasts
      Statistics m_lastStat; ///< Current object to be broadcast
      std::list< Address > m_addresses; ///< Addresses which are listening for statistics
      mutable msg::Logger m_logger; ///< Private logger

   }; // class Sender

} // namespace cdastat

#endif // CDA_DAQ_STAT_SENDER_H
