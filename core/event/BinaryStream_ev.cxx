// $Id$
//
// The file is called BinaryStream_ev.cxx, since there is a class called
// BinaryStream in the msg namespace as well. If the two source files
// would be called the same, qmake would have difficulties compiling the
// library...
//

// CDA include(s):
#include "../common/errorcheck.h"

// Local include(s):
#include "BinaryStream.h"

namespace ev {

   BinaryStream::BinaryStream( QIODevice* device )
      : QDataStream( device ),
        m_logger( "ev::BinaryStream" ) {

   }

   BinaryStream::BinaryStream( QByteArray* array,
                               QIODevice::OpenMode openMode )
      : QDataStream( array, openMode ),
        m_logger( "ev::BinaryStream" ) {

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

      const std::vector< std::tr1::shared_ptr< Fragment > >& fragments =
         event.getFragments();

      ( * ( QDataStream* ) this ) << ( quint32 ) fragments.size();

      for( size_t i = 0; i < fragments.size(); ++i ) {
         *this << *( fragments[ i ] );
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

      if( ! ensureDataAvailable( 4 ) ) {
         REPORT_ERROR( tr( "Can't read the needed data" ) );
         return *this;
      }
      quint32 nFragments;
      ( * ( QDataStream* ) this ) >> nFragments;

      for( quint32 i = 0; i < nFragments; ++i ) {
         Fragment* fragment = new Fragment();
         *this >> *fragment;
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

      const std::vector< uint32_t >& dwords = fragment.getDataWords();

      ( * ( QDataStream* ) this ) << fragment.getModuleID();
      ( * ( QDataStream* ) this ) << ( quint32 ) dwords.size();

      for( size_t i = 0; i < dwords.size(); ++i ) {
         ( * ( QDataStream* ) this ) << dwords[ i ];
         if( ! ensureDataSent( 10000 ) ) {
            REPORT_ERROR( tr( "Couldn't send data" ) );
            return *this;
         }
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

      int moduleID;
      quint32 nDataWords;

      if( ! ensureDataAvailable( 8 ) ) {
         REPORT_ERROR( tr( "Can't read the needed data" ) );
         return *this;
      }
      ( * ( QDataStream* ) this ) >> moduleID;
      ( * ( QDataStream* ) this ) >> nDataWords;

      fragment.setModuleID( moduleID );

      uint32_t dataWord;
      for( quint32 i = 0; i < nDataWords; ++i ) {
         if( ! ensureDataAvailable( 4 ) ) {
            REPORT_ERROR( tr( "Can't read the needed data" ) );
            return *this;
         }
         ( * ( QDataStream* ) this ) >> dataWord;
         fragment.addDataWord( dataWord );
      }

      return *this;
   }

   bool BinaryStream::ensureDataAvailable( qint64 minBytes ) {

      // If we are not operating on an I/O device, then
      // return right away:
      if( ! device() ) return true;

      // Check if the requested amount of data is available:
      if( device()->bytesAvailable() >= minBytes ) return true;

      // Wait for new data to arrive to the socket:
      CHECK( device()->waitForReadyRead( 500 ) );

      return true;
   }

   bool BinaryStream::ensureDataSent( qint64 maxBytes ) {

      // If we are not operating on an I/O device, then
      // return right away:
      if( ! device() ) return true;

      // Check if the requested amount of data is available:
      if( device()->bytesToWrite() < maxBytes ) return true;

      // Write new data to the output:
      CHECK( device()->waitForBytesWritten( 500 ) );

      return true;
   }

} // namespace ev
