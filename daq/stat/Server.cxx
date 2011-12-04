// $Id$

// CDA include(s):
#include <QtCore/QtGlobal>
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Address.h"
#   include "cdacore/common/Socket.h"
#else
#   include "common/Address.h"
#   include "common/Socket.h"
#endif

// Local include(s):
#include "Server.h"
#include "BinaryStream.h"

namespace cdastat {

   /**
    * @param parent Qt parent of this object
    */
   Server::Server( QObject* parent )
      : QTcpServer( parent ) {

   }

   /**
    * @param address The address where it should listen to incoming stat info
    * @returns <code>true</code> on success,
    *          <code>false</code> otherwise
    */
   bool Server::listen( const Address& address ) {

      return QTcpServer::listen( address.getHost(), address.getPort() );

   }

   /**
    * @param source Socket that has some new data
    */
   void Server::readStat( Socket* source ) {

      // Small security check:
      if( source->bytesAvailable() < 4 ) return;

      // Create the object that will be distributed:
      Statistics stat;

      // Read the message from the socket:
      BinaryStream in( source );
      in >> stat;

      // Distribute the object within the application:
      emit statAvailable( stat );

      return;

   }

   /**
    * This reimplemented function creates CDA's own Socket object to handle
    * all incoming connections, and connects the signals of this Socket
    * object to various slots.
    */
   void Server::incomingConnection( int socketDescriptor ) {

      Socket* socket = new Socket( this );
      socket->setSocketDescriptor( socketDescriptor );

      connect( socket, SIGNAL( dataReady( Socket* ) ),
               this, SLOT( readStat( Socket* ) ) );
      connect( socket, SIGNAL( disconnected() ),
               socket, SLOT( deleteLater() ) );

      return;

   }

} // namespace cdastat
