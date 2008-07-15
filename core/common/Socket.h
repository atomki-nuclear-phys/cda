// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_SOCKET_H
#define CDA_CORE_COMMON_SOCKET_H

// Qt include(s):
#include <QtNetwork/QTcpSocket>

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "msg::Socket needs Qt >= 4.0.0"
#endif

/**
 *  @short Network socket for transmitting data
 *
 *         Since I had a lot of trouble using the interface provided
 *         by QTcpSocket for reading data with a high speed, I
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
   /// Special signal showing that data is ready to be read
   void dataReady( Socket* socket );

protected slots:
   /// Internal slot for producing the dataReady signal
   void handleDataReady();

}; // class Socket

#endif // CDA_CORE_COMMON_SOCKET_H
