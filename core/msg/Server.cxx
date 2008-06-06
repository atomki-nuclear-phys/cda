// $Id$

// Qt include(s):
#include <QtCore/QFile>

// Local include(s):
#include "Server.h"
#include "Socket.h"
#include "BinaryStream.h"
#include "TextStream.h"

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QFile );

   /**
    * The constructor sets all the properties of the object to sensible
    * defaults. This is: An output file is written with the name
    * <strong>messages.log</strong> with the incoming messages.
    *
    * @param parent The Qt parent of the object
    */
   Server::Server( QObject* parent )
      : QTcpServer( parent ), m_writeOutputFile( true ),
        m_outputFileName( "messages.log" ) {

   }

   /**
    * @param value <code>true</code> if an output file should be written,
    *              <code>false</code> otherwise
    */
   void Server::setWriteOutputFile( bool value ) {

      m_writeOutputFile = value;
      return;

   }

   /**
    * @param name The name of the output file
    */
   void Server::setOutputFileName( const QString& name ) {

      m_outputFileName = name;
      return;

   }

   /**
    * @returns <code>true</code> if an output file should be written,
    *          <code>false</code> otherwise
    */
   bool Server::getWriteOutputFile() const {

      return m_writeOutputFile;

   }

   /**
    * @returns The name of the output file
    */
   const QString& Server::getOutputFileName() const {

      return m_outputFileName;

   }

   /**
    * @param address The address on which the server should listen for
    *                incoming messages
    */
   bool Server::listen( const Address& address ) {

      return QTcpServer::listen( address.getHost(), address.getPort() );

   }

   /**
    * This function is responsible for reading the messages from the
    * network. It does this using msg::ByteStream to decode the
    * incoming data.
    *
    * Upon successfuly reading a message, it writes the message to the
    * specified output file if it is set up accordingly, and finally
    * emits the <code>messageAvailable</code> signal.
    *
    * @param source The connection on which a message can be read
    */
   void Server::readMessage( Socket* source ) {

      // Small security check:
      if( source->bytesAvailable() < 4 ) return;

      Message message;

      // Read the message from the socket:
      BinaryStream in( source );
      in >> message;

      // If configured, write an entry to the log file:
      if( m_writeOutputFile ) {
         QFile file( m_outputFileName );
         file.open( QIODevice::Append );
         TextStream out( &file );
         out << message << "\n";
      }

      // Tell the world that a new message is available:
      emit messageAvailable( message );

      return;

   }

   /**
    * This function from the base class is re-implemented here to take
    * advantage of the additional feature(s) available in
    * msg::Socket and change the behaviour of the server a little.
    *
    * @param socketDescriptor The socket descriptor on which a message
    *                         can be read
    */
   void Server::incomingConnection( int socketDescriptor ) {

      Socket* socket = new Socket( this );
      socket->setSocketDescriptor( socketDescriptor );

      connect( socket, SIGNAL( messageReady( Socket* ) ),
               this, SLOT( readMessage( Socket* ) ) );
      connect( socket, SIGNAL( disconnected() ),
               socket, SLOT( deleteLater() ) );

      return;

   }

} // namespace msg
