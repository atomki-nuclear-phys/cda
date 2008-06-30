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

   BinaryStream::~BinaryStream() {

   }

   BinaryStream& BinaryStream::operator<< ( const Event& event ) {

      setVersion( QDataStream::Qt_4_4 );
      ( * ( QDataStream* ) this ) << ( quint32 ) event.getFragments().size();

      for( std::vector< Fragment >::const_iterator fragment =
              event.getFragments().begin();
           fragment != event.getFragments().end(); ++fragment ) {
         *this << *fragment;
      }

      return *this;

   }

   BinaryStream& BinaryStream::operator>> ( Event& event ) {

      setVersion( QDataStream::Qt_4_4 );

      quint32 nFragments;
      ( * ( QDataStream* ) this ) >> nFragments;

      for( quint32 i = 0; i < nFragments; ++i ) {
         Fragment fragment;
         *this >> fragment;
         event.addFragment( fragment );
      }

      return *this;

   }

   BinaryStream& BinaryStream::operator<< ( const Fragment& fragment ) {

      setVersion( QDataStream::Qt_4_4 );
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

   BinaryStream& BinaryStream::operator>> ( Fragment& fragment ) {

      setVersion( QDataStream::Qt_4_4 );

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
