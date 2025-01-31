// Dear emacs, this is -*- c++ -*-
/**
 *   @file core/common/BinaryStream.h
 *  @short Header file declaring the BinaryStream class
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
#ifndef CDA_CORE_COMMON_BINARYSTREAM_H
#define CDA_CORE_COMMON_BINARYSTREAM_H

// Qt include(s):
#include <QDataStream>

// Local include(s):
#include "Address.h"
#include "Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QIODevice)
QT_FORWARD_DECLARE_CLASS(QByteArray)

/**
 *  @short Binary stream serialising Address objects
 *
 *         The need presented itself to read/write Address objects
 *         from/to binary data. This class takes care of this translation.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDACORE_EXPORT BinaryStream : public QDataStream {

public:
   /// Standard QDataStream constructor
   BinaryStream(QIODevice* device = 0);
   /// Constructor I use mainly for debugging
   BinaryStream(QByteArray* array,
                QIODevice::OpenMode openMode = QIODevice::ReadWrite);

   /// Operator writing an Address object in binary format
   BinaryStream& operator<<(const Address& address);
   /// Operator reading an Address object from binary input
   BinaryStream& operator>>(Address& address);

};  // class BinaryStream

#endif  // CDA_CORE_COMMON_BINARYSTREAM_H
