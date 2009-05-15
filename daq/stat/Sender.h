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
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Statistics.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QTcpSocket )
namespace msg {
   class Address;
}

namespace stat {

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
   class Sender {

      // To have the tr() function:
      Q_DECLARE_TR_FUNCTIONS( stat::Sender )

   public:
      /// Constructor
      Sender();
      /// Destructor
      ~Sender();

      /// Add an address where statistics information should be sent to
      bool addReceiver( const Address& address );

      /// Send statistics to all the configured receivers
      bool send( const Statistics& stat ) const;

   private:
      /// Print a connection error message
      void printError( const QTcpSocket& socket ) const;

      std::list< QTcpSocket* > m_sockets; ///< List of sockets where statistics should be sent to
      mutable msg::Logger m_logger;       ///< Private logger

   }; // class Sender

} // namespace stat

#endif // CDA_DAQ_STAT_SENDER_H
