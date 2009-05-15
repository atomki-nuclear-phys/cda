// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_SERVER_H
#define CDA_DAQ_STAT_SERVER_H

// Qt include(s):
#include <QtNetwork/QTcpServer>

// Local include(s):
#include "Statistics.h"

// Forward declaration(s):
class Address;
class Socket;

namespace stat {

   /**
    *  @short Statistics server, listening to incoming statistics info
    *
    *         The naming of this class is quite misleading actually. It
    *         doesn't serve other applications with statistics information,
    *         instead it listens to incoming statistics information. I only
    *         named it like this, since msg::Server and ev::Server are
    *         written in such a manner as well...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Server : public QTcpServer {

      Q_OBJECT

   public:
      /// Constructor
      Server( QObject* parent = 0 );

      /// Listen to statistics information on the given address
      bool listen( const Address& address );

   signals:
      /// Signal emitted when statistic info is available
      void statAvailable( const stat::Statistics& stat );

   protected slots:
      /// Slot reading the statistics information
      void readStat( Socket* source );

   protected:
      /// Reimplemented function handling incoming connections
      virtual void incomingConnection( int socketDescriptor );

   }; // class Server

} // namespace stat

#endif // CDA_DAQ_STAT_SERVER_H