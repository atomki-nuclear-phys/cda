// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_SERVER_H
#define CDA_CORE_MSG_SERVER_H

// Qt include(s):
#include <QtNetwork/QTcpServer>
#include <QtCore/QString>

// Local include(s):
#include "Message.h"
#include "Address.h"

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "msg::Server needs Qt >= 4.0.0"
#endif

namespace msg {

   // Forward declaration(s):
   class Socket;

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QTcpServer );
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QObject );

   /**
    *  @short Message server accepting incoming messages
    *
    *         This class can be used to implement an executable
    *         that acts as a message server. Since the class
    *         inherits from QTcpServer, it can be controlled with
    *         the same commands. Most importantly, it can be started
    *         to listen to incoming connections with the
    *
    *         <code>msg::Server::listen(...)</code>
    *
    *         function. Currently it can do two things. It can
    *         write the incoming messages to a log file in a nicely
    *         formatted fashion, and it can store the messages
    *         internally for come client code to read them out.
    *         (For instance to show the messages in a graphical
    *         window.)
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Server : public QTcpServer {

      Q_OBJECT

   public:
      /// Standard constructor
      Server( QObject* parent = 0 );

      /// Set whether an output file should be written
      void setWriteOutputFile( bool value );
      /// Set the name of the output file
      void setOutputFileName( const QString& name );

      /// Tells you if an output file is being written
      bool           getWriteOutputFile() const;
      /// Tells you the name of the output file
      const QString& getOutputFileName() const;

      bool listen( const Address& address );

   signals:
      /// Is emitted when a new message becomes available for readout
      void messageAvailable( const Message& message );

   protected slots:
      /// Function used internally to read the messages from the net
      void readMessage( Socket* source );

   protected:
      /// Re-implemented function from the base class
      virtual void incomingConnection( int socketDescriptor );

   private:
      bool    m_writeOutputFile; ///< Switch for writing an output file
      QString m_outputFileName;  ///< Output file name

   }; // class Server

} // namespace msg

#endif // CDA_CORE_MSG_SERVER_H
