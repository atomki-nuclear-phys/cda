// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_BINARYSTREAM_H
#define CDA_CORE_COMMON_BINARYSTREAM_H

// Qt include(s):
#include <QtCore/QDataStream>

// Local include(s):
#include "Address.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QByteArray )

class BinaryStream : public QDataStream {

public:
   BinaryStream( QIODevice* device = 0 );
   BinaryStream( QByteArray* array,
                 QIODevice::OpenMode openMode = QIODevice::ReadWrite );

   BinaryStream& operator<< ( const Address& address );
   BinaryStream& operator>> ( Address& address );

}; // class BinaryStream

#endif // CDA_CORE_COMMON_BINARYSTREAM_H
