// $Id$

// Local include(s):
#include "BinaryStream.h"
#include "Statistics.h"

namespace stat {

   BinaryStream::BinaryStream( QIODevice* device )
      : QDataStream( device ) {

      setVersion( QDataStream::Qt_4_0 );

   }

   BinaryStream::BinaryStream( QByteArray* array, QIODevice::OpenMode openMode )
      : QDataStream( array, openMode ) {

      setVersion( QDataStream::Qt_4_0 );

   }

   BinaryStream& BinaryStream::operator<< ( const Statistics& stat ) {

      setVersion( QDataStream::Qt_4_0 );

      ( * ( QDataStream* ) this ) << stat.getProcessedEvents();
      ( * ( QDataStream* ) this ) << stat.getStatTime();

      return *this;

   }

   BinaryStream& BinaryStream::operator>> ( Statistics& stat ) {

      setVersion( QDataStream::Qt_4_0 );

      quint32 events;
      QTime statTime;

      ( * ( QDataStream* ) this ) >> events;
      ( * ( QDataStream* ) this ) >> statTime;

      stat.setProcessedEvents( events );
      stat.setStatTime( statTime );

      return *this;

   }

} // namespace stat
