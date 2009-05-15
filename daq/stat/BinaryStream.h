// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_STAT_BINARYSTREAM_H
#define CDA_DAQ_STAT_BINARYSTREAM_H\

// Qt include(s):
#include <QtCore/QDataStream>

namespace stat {

   // Forward declaration(s):
   class Statistics;

   /**
    *  @short Class (de-)serialising statistics objects
    *
    *         This class is used internally by the classes sending statistics
    *         information over the network, to convert statistics objects
    *         to/from binary data.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class BinaryStream : public QDataStream {

   public:
      /// Constructor receiving an I/O device
      BinaryStream( QIODevice* device = 0 );
      /// Constructor receiving a byte-array
      BinaryStream( QByteArray* array,
                    QIODevice::OpenMode openMode = QIODevice::ReadWrite );

      /// Operator serialising statistics objects
      BinaryStream& operator<< ( const Statistics& stat );
      /// Operator de-serialising statistics objects
      BinaryStream& operator>> ( Statistics& stat );

   }; // class BinaryStream

} // namespace stat

#endif // CDA_DAQ_STAT_BINARYSTREAM_H
