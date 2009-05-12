// $Id$

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/common/Socket.h"
#else
#   include "common/Socket.h"
#endif

// Local include(s):
#include "Server.h"
#include "BinaryStream.h"

namespace stat {

   Server::Server( QObject* parent )
      : QTcpServer( parent ) {

   }

   bool Server::listen( const Address& address ) {

      return QTcpServer::listen( address.getHost(), address.getPort() );

   }

   void Server::readStat( Socket* source ) {

      // Small security check:
      if( source->bytesAvailable() < 4 ) return;

      Statistics stat;

      // Read the message from the socket:
      BinaryStream in( source );
      in >> stat;

      emit statAvailable( stat );

   }

   void Server::incomingConnection( int socketDescriptor ) {

      Socket* socket = new Socket( this );
      socket->setSocketDescriptor( socketDescriptor );

      connect( socket, SIGNAL( dataReady( Socket* ) ),
               this, SLOT( readStat( Socket* ) ) );
      connect( socket, SIGNAL( disconnected() ),
               socket, SLOT( deleteLater() ) );

      return;

   }

} // namespace stat
