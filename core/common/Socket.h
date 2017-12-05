// Dear emacs, this is -*- c++ -*-
/**
 *   @file core/common/Socket.h
 *  @short Header file declaring the Socket class
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
#ifndef CDA_CORE_COMMON_SOCKET_H
#define CDA_CORE_COMMON_SOCKET_H

// Qt include(s):
#include <QTcpSocket>

// CDA include(s):
#include "Export.h"

/**
 *  @short Network socket for transmitting data
 *
 *         Since I had a lot of trouble using the interface provided
 *         by QTcpSocket for reading data with a high speed, I
 *         had to extend its interface a bit. The extension is
 *         rather simple, but it makes a huge difference in
 *         reliability...
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDACORE_EXPORT Socket : public QTcpSocket {

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
