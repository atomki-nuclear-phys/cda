
// CDA include(s):
#include "../common/errorcheck.h"

// Local include(s):
#include "BinaryStream.h"
#include "Event.h"
#include "Fragment.h"

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

      ( * ( QDataStream* ) this ) << ( quint32 ) event.getFragments().size();

      for( auto& fragment : event.getFragments() ) {
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
   BinaryStream&
   BinaryStream::operator>> ( Event& event ) {

      event.clear();

      setVersion( QDataStream::Qt_4_0 );

      if( ! ensureDataAvailable( 4 ) ) {
         m_logger << msg::WARNING
                  << tr( "Incomplete event received" )
                  << msg::endmsg;
         throw IncompleteEvent();
         return *this;
      }
      quint32 nFragments;
      ( * ( QDataStream* ) this ) >> nFragments;

      for( quint32 i = 0; i < nFragments; ++i ) {
         std::unique_ptr< Fragment > fragment( new Fragment() );
         *this >> *fragment;
         event.addFragment( std::move( fragment ) );
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

      const Fragment::Payload_t& dwords = fragment.getDataWords();

      ( * ( QDataStream* ) this ) << fragment.getModuleID();
      ( * ( QDataStream* ) this ) << ( quint32 ) dwords.size();

      for( size_t i = 0; i < dwords.size(); ++i ) {
         ( * ( QDataStream* ) this ) << dwords[ i ];
         if( ! ensureDataSent() ) {
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
   BinaryStream&
   BinaryStream::operator>> ( Fragment& fragment ) {

      fragment.clear();

      setVersion( QDataStream::Qt_4_0 );

      Fragment::Identifier_t moduleID;
      quint32 nDataWords;

      if( ! ensureDataAvailable( sizeof( moduleID ) +
                                 sizeof( nDataWords ) ) ) {
         m_logger << msg::WARNING
                  << tr( "Incomplete event received" )
                  << msg::endmsg;
         throw IncompleteEvent();
         return *this;
      }
      ( * ( QDataStream* ) this ) >> moduleID;
      ( * ( QDataStream* ) this ) >> nDataWords;

      fragment.setModuleID( moduleID );

      Fragment::Payload_t::value_type dataWord;
      for( quint32 i = 0; i < nDataWords; ++i ) {
         if( ! ensureDataAvailable( sizeof( dataWord ) ) ) {
            m_logger << msg::WARNING
                     << tr( "Incomplete event received" )
                     << msg::endmsg;
            throw IncompleteEvent();
            return *this;
         }
         ( * ( QDataStream* ) this ) >> dataWord;
         fragment.addDataWord( dataWord );
      }

      return *this;
   }

   /**
    * A large event may end up in multiple TCP windows, which means that
    * it will need to be read in multiple steps by the data processing
    * application. This function makes sure that the socket has data
    * ready for the upcoming read request(s).
    *
    * @param minBytes Bytes needed for the next read operation(s)
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
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

   /**
    * As I've found out, the TCP windows have a maximum size of 64 kB.
    * Some of the devices in CDA can produce events that are larger than
    * this. (I'm looking at you, DT5740...)
    *
    * When this happens, the code needs to send the event in multiple
    * TCP windows. This function makes sure than when there are more than
    * 50 kB of data ready to be written, they do get written to the
    * socket.
    *
    * @param maxBytes Maximum number of bytes for writing at once
    * @returns <code>true</code> if the operation was successful,
    *          <code>false</code> otherwise
    */
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
