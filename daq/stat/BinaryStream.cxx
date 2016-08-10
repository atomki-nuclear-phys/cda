// $Id$

// Local include(s):
#include "BinaryStream.h"
#include "Statistics.h"

namespace cdastat {

   BinaryStream::BinaryStream( QIODevice* device )
      : QDataStream( device ) {

      setVersion( QDataStream::Qt_4_0 );
   }

   BinaryStream::BinaryStream( QByteArray* array, QIODevice::OpenMode openMode )
      : QDataStream( array, openMode ) {

      setVersion( QDataStream::Qt_4_0 );
   }

   /**
    * This operator translates all the data stored in a stat::Statistics object
    * into simple binary data. It does this by using Qt's ability to serialise
    * its own objects/
    *
    * @param stat The statistics object to serialise
    * @returns This same stream object
    */
   BinaryStream& BinaryStream::operator<< ( const Statistics& stat ) {

      setVersion( QDataStream::Qt_4_0 );

      ( * ( QDataStream* ) this ) << stat.getProcessedEvents();
      ( * ( QDataStream* ) this ) << stat.getLostEvents();
      ( * ( QDataStream* ) this ) << stat.getSource();
      ( * ( QDataStream* ) this ) << stat.getStatTime();

      return *this;
   }

   /**
    * This operator reads data from the input and translates it into the
    * properties of a stat::Statistics object.
    *
    * @param stat Object to fill with the data from the input stream
    * @returns This same stream object
    */
   BinaryStream& BinaryStream::operator>> ( Statistics& stat ) {

      setVersion( QDataStream::Qt_4_0 );

      quint32 processedEvents = 0, lostEvents = 0;
      QString source;
      QTime statTime;

      ( * ( QDataStream* ) this ) >> processedEvents;
      ( * ( QDataStream* ) this ) >> lostEvents;
      ( * ( QDataStream* ) this ) >> source;
      ( * ( QDataStream* ) this ) >> statTime;

      stat.setProcessedEvents( processedEvents );
      stat.setLostEvents( lostEvents );
      stat.setSource( source );
      stat.setStatTime( statTime );

      return *this;
   }

} // namespace cdastat
