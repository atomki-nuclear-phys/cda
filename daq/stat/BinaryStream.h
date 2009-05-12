// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_BINARYSTREAM_H
#define CDA_DAQ_STAT_BINARYSTREAM_H\

// Qt include(s):
#include <QtCore/QDataStream>

namespace stat {

   // Forward declaration(s):
   class Statistics;

   class BinaryStream : public QDataStream {

   public:
      BinaryStream( QIODevice* device = 0 );
      BinaryStream( QByteArray* array,
                    QIODevice::OpenMode openMode = QIODevice::ReadWrite );

      BinaryStream& operator<< ( const Statistics& stat );
      BinaryStream& operator>> ( Statistics& stat );

   }; // class BinaryStream

} // namespace stat

#endif // CDA_DAQ_STAT_BINARYSTREAM_H
