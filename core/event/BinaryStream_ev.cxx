// $Id$
//
// The file is called BinaryStream_ev.cxx, since there is a class called
// BinaryStream in the msg namespace as well. If the two source files
// would be called the same, qmake would have difficulties compiling the
// library...
//

// Local include(s):
#include "BinaryStream.h"

namespace ev {

   BinaryStream::BinaryStream( QIODevice* device )
      : QDataStream( device ) {

   }

   BinaryStream::BinaryStream( QByteArray* array,
                               QIODevice::OpenMode openMode )
      : QDataStream( array, openMode ) {

   }

   /**
    * The serialisation of an event is pretty simple. First the
    * number of event fragments in written to the stream, then each
    * fragment is serialised one after the other.
    *
    * @returns This same object
    */
   BinaryStream& BinaryStream::operator<< ( const Event& event ) {

      setVersion( QDataStream::Qt_4_0 );

      ( * ( QDataStream* ) this ) << ( quint32 ) event.getFragments().size();

      for( std::vector< Fragment >::const_iterator fragment =
              event.getFragments().begin();
           fragment != event.getFragments().end(); ++fragment ) {
         *this << *fragment;
      }

      return *this;
   }

   /**
    * The de-serialisation is quite simple as well. First it reads
    * how many event fragments it should expect, then it tries to
    * read that many event fragments and add them to the event
    * object given to the operator.
    *
    * @returns This same object
    */
   BinaryStream& BinaryStream::operator>> ( Event& event ) {

      event.clear();

      setVersion( QDataStream::Qt_4_0 );

      quint32 nFragments;
      ( * ( QDataStream* ) this ) >> nFragments;

      for( quint32 i = 0; i < nFragments; ++i ) {
         Fragment fragment;
         *this >> fragment;
         event.addFragment( fragment );
      }

      return *this;
   }

   /**
    * Serialising an event fragment is a bit longer in code, but not
    * more difficult. First the simple members of the class are written
    * to the stream. Then the number of data words is written followed
    * by the data words themselves.
    *
    * @returns This same object
    */
   BinaryStream& BinaryStream::operator<< ( const Fragment& fragment ) {

      setVersion( QDataStream::Qt_4_0 );

      ( * ( QDataStream* ) this ) << fragment.getCrateNumber();
      ( * ( QDataStream* ) this ) << fragment.getModuleNumber();
      ( * ( QDataStream* ) this ) << ( quint32 ) fragment.getDataWords().size();

      for( std::vector< uint32_t >::const_iterator it =
              fragment.getDataWords().begin();
           it != fragment.getDataWords().end(); ++it ) {
         ( * ( QDataStream* ) this ) << ( quint32 ) ( *it );
      }

      return *this;
   }

   /**
    * The operator reads the simple members of the event fragment in the
    * same order that they were serialised in, then reads the number of
    * data words that it should expect, finally reads that number of
    * data words. Pretty simple actually...
    *
    * @returns This same object
    */
   BinaryStream& BinaryStream::operator>> ( Fragment& fragment ) {

      fragment.clear();

      setVersion( QDataStream::Qt_4_0 );

      int crateNumber;
      int moduleNumber;
      quint32 nDataWords;

      ( * ( QDataStream* ) this ) >> crateNumber;
      ( * ( QDataStream* ) this ) >> moduleNumber;
      ( * ( QDataStream* ) this ) >> nDataWords;

      fragment.setCrateNumber( crateNumber );
      fragment.setModuleNumber( moduleNumber );

      uint32_t dataWord;
      for( quint32 i = 0; i < nDataWords; ++i ) {
         ( * ( QDataStream* ) this ) >> dataWord;
         fragment.addDataWord( dataWord );
      }

      return *this;
   }

} // namespace ev
