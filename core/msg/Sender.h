// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_SENDER_H
#define CDA_CORE_MSG_SENDER_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "../common/Address.h"
#include "../common/Export.h"

// Local include(s):
#include "Level.h"
#include "Message.h"

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QHostAddress );

   /**
    *  @short Singleton class sending messages over the network
    *
    *         Every application only needs one instance of this object,
    *         hence it is implemented as a singleton. A connection has
    *         to be opened to the server every time a new message is sent,
    *         because for some reason I couldn't get Qt to hold a TCP
    *         connection properly open for me... :-/
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT Sender {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( msg::Sender )

   public:
      /// Function to access the singleton instance with
      static Sender* instance();
      /// Function adding a new destination for the messages
      static void addAddress( const Address& address );
      /// Get all the addresses the messages are sent to
      static const std::vector< Address >& getAddresses();

      /// Get the message type above which messages are sent
      Level getMinLevel() const;
      /// Set the message type above which messages are sent
      void setMinLevel( Level level );

      /// Function sending a message to the server
      void send( const Message& message ) const;

   protected:
      /// The constructor is protected to enforce the singleton design
      Sender();

   private:
      /// Function for communicating a message transmission failiure
      void printError( const Message& message ) const;

      static Sender* m_instance; ///< The only instance of the object
      static std::vector< Address > m_addresses; ///< Message server addresses

      Level m_minLevel; ///< Messages below this level are not transmitted

   }; // class Sender

} // namespace msg

#endif // CDA_CORE_MSG_SENDER_H
