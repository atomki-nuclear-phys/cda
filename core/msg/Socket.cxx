// $Id$

// Local include(s):
#include "Socket.h"

namespace msg {

   /**
    * The constructor sets up the triggering of the messageReady
    * signal.
    *
    * @param parent The Qt parent of the socket
    */
   Socket::Socket( QObject* parent )
      : QTcpSocket( parent ) {

      connect( this, SIGNAL( readyRead() ),
               this, SLOT( handleMessageReady() ) );

   }

   /**
    * The base QTcpSocket class can only send signals that say that
    * "a" socket has new data to read out. If the server has a lot of
    * concurrent connections, it can be a real pain to figure out which
    * socket sent the signal.
    *
    * This class helps by emitting a messageReady signal every time the
    * base class emits a readyRead signal. The trick is that messageReady
    * specifies to the receiving slot exactly which socket emitted it.
    */
   void Socket::handleMessageReady() {

      emit messageReady( this );
      return;

   }

} // namespace msg
