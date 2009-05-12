// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_SENDER_H
#define CDA_DAQ_STAT_SENDER_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QtCore/QCoreApplication>

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

namespace stat {

   using QT_PREPEND_NAMESPACE( QTcpSocket );

   class Sender {

      // To have the tr() function:
      Q_DECLARE_TR_FUNCTIONS( stat::Sender )

   public:
      Sender();
      ~Sender();

      bool addReceiver( const Address& address );

      bool send( const Statistics& stat ) const;

   private:
      void printError( const QTcpSocket& socket ) const;

      std::list< QTcpSocket* > m_sockets;
      mutable msg::Logger m_logger;

   }; // class Sender

} // namespace stat

#endif // CDA_DAQ_STAT_SENDER_H
