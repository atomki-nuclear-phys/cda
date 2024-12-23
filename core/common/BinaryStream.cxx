/**
 *   @file core/common/BinaryStream.cxx
 *  @short File implementing the BinaryStream class
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */

// Local include(s):
#include "BinaryStream.h"

BinaryStream::BinaryStream(QIODevice* device) : QDataStream(device) {

   setVersion(QDataStream::Qt_4_0);
}

BinaryStream::BinaryStream(QByteArray* array, QIODevice::OpenMode openMode)
    : QDataStream(array, openMode) {

   setVersion(QDataStream::Qt_4_0);
}

BinaryStream& BinaryStream::operator<<(const Address& address) {

   setVersion(QDataStream::Qt_4_0);
   (*(QDataStream*)this) << address.getHost();
   (*(QDataStream*)this) << address.getPort();

   return *this;
}

BinaryStream& BinaryStream::operator>>(Address& address) {

   QHostAddress host;
   quint16 port;

   setVersion(QDataStream::Qt_4_0);
   (*(QDataStream*)this) >> host;
   (*(QDataStream*)this) >> port;

   address.setHost(host);
   address.setPort(port);

   return *this;
}
