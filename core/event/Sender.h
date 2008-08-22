// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_SENDER_H
#define CDA_CORE_EVENT_SENDER_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QTcpSocket>
// CDA include(s):
#include "../common/Address.h"
#include "../msg/Logger.h"

// Local include(s):
#include "Event.h"

namespace ev {

   /**
    *  @short Class for sending events over the network
    *
    *         This class can be used to send full events over a TCP
    *         network to a variable number of destinations. This class
    *         should be used in conjunction with ev::EventServer to
    *         send events over the network.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Sender {

      // To have the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ev::Sender )

   public:
      /// Constructor
      Sender();
      /// Destructor
      ~Sender();

      /// Add an address to the "recipient list"
      void addSocket(QTcpSocket &socket);
      bool addSocket( const Address& address );
      bool addSocket( const QHostAddress& address,const quint16 port);  

      /// Function for sending a message to the recipients
      bool send( const Event& event ) const;

   private:
      /// Internal function for printing error messages
      void printError( const QTcpSocket& socket ) const;
      std::list< QTcpSocket* > m_sockets;
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Sender

} // namespace ev

#endif // CDA_CORE_EVENT_SENDER_H
