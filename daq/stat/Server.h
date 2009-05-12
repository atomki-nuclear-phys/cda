// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_SERVER_H
#define CDA_DAQ_STAT_SERVER_H

// Qt include(s):
#include <QtNetwork/QTcpServer>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#else
#   include "common/Address.h"
#endif

// Local include(s):
#include "Statistics.h"

// Forward declaration(s):
class Socket;

namespace stat {

   class Server : public QTcpServer {

      Q_OBJECT

   public:
      Server( QObject* parent = 0 );

      bool listen( const Address& address );

   signals:
      void statAvailable( const stat::Statistics& stat );

   protected slots:
      void readStat( Socket* source );

   protected:
      virtual void incomingConnection( int socketDescriptor );

   }; // class Server

} // namespace stat

#endif // CDA_DAQ_STAT_SERVER_H
