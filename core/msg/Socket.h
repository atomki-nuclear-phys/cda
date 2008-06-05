// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_SOCKET_H
#define CDA_CORE_MSG_SOCKET_H

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "MSG::Socket needs Qt >= 4.0.0"
#endif

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // MSG namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QTcpSocket );
   using QT_PREPEND_NAMESPACE( QObject );

   /**
    *  @short Network socket for transmitting messages
    *
    *         Since I had a lot of trouble using the interface provided
    *         by QTcpSocket for reading messages with a high speed, I
    *         had to extend its interface a bit. The extension is
    *         rather simple, but it makes a huge difference in
    *         reliability...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Socket : public QTcpSocket {

      Q_OBJECT

   public:
      /// Standard constructor
      Socket( QObject* parent = 0 );

   signals:
      /// Special signal showing that a message is ready to be read
      void messageReady( Socket* socket );

   protected slots:
      /// Internal slot for producing the messageReady signal
      void handleMessageReady();

   }; // class Socket

} // namespace msg

#endif // CDA_CORE_MSG_SOCKET_H
