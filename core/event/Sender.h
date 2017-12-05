// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_EVENT_SENDER_H
#define CDA_CORE_EVENT_SENDER_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QCoreApplication>

// CDA include(s):
#include "../common/Address.h"
#include "../common/Export.h"
#include "../msg/Logger.h"

// Local include(s):
#include "Event.h"
#include "BinaryStream.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QTcpSocket )

namespace ev {

   //
   // Make sure that the following Qt classes are available in the
   // ev namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QTcpSocket );

   /**
    *  @short Class for sending events over the network
    *
    *         This class can be used to send full events over a TCP
    *         network to a variable number of destinations. This class
    *         should be used in conjunction with ev::EventServer to
    *         send events over the network.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDACORE_EXPORT Sender {

      // To have the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ev::Sender )

   public:
      /// Constructor
      Sender();
      /// Destructor
      ~Sender();

      /// Add an address to the "recipient list"
      bool addSocket( const Address& address );

      /// Function for sending a message to the recipients
      bool send( const Event& event ) const;

   private:
      /// Internal function for printing error messages
      void printError( const QTcpSocket& socket ) const;

      std::list< QTcpSocket* > m_sockets; ///< List of sockets to send events to
      std::list< BinaryStream* > m_streams; ///< List of streams to send events to
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Sender

} // namespace ev

#endif // CDA_CORE_EVENT_SENDER_H
