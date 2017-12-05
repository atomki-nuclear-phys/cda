
// Qt include(s):
#include <QIODevice>
#include <QByteArray>

// Local include(s):
#include "BinaryStream.h"

namespace msg {

   /**
    * This constructor can be used to transmit messages over the
    * network.
    *
    * @param device Any kind of object inheriting from (and implementing) QIODevice
    */
   BinaryStream::BinaryStream( QIODevice* device )
      : QDataStream( device ) {

      this->initMaps();
      setVersion( QDataStream::Qt_4_0 );
   }

   /**
    * This constructor is mainly used for testing purposes.
    *
    * @param array A QByteArray object where you want to write to/read from
    * @param openMode Mode to operate on the specified QByteArray
    */
   BinaryStream::BinaryStream( QByteArray* array,
                               QIODevice::OpenMode openMode )
      : QDataStream( array, openMode ) {

      this->initMaps();
   }

   /**
    * This function encodes a msg::Message object to a binary form
    * that can be transmitted over the network, written to a file,
    * you name it.
    *
    * The implementation relies heavily on the QDataStream parent of
    * the class, so it's relatively simple.
    *
    * @param message The Message object to write to the QIODevice
    * @returns This same object
    */
   BinaryStream& BinaryStream::operator<< ( const Message& message ) {

      //
      // Encode the message level:
      //
      std::map< Level, quint16 >::const_iterator level_it;
      quint16 level_num;
      if( ( level_it = m_forwardMap.find( message.getLevel() ) ) ==
          m_forwardMap.end() ) {
         // This is a bad level!
         level_num = 3;
      } else {
         level_num = level_it->second;
      }

      //
      // Encode the msg::Message parameters in a specific order:
      //
      setVersion( QDataStream::Qt_4_4 );
      ( * ( QDataStream* ) this ) << message.getSender();
      ( * ( QDataStream* ) this ) << message.getText();
      ( * ( QDataStream* ) this ) << level_num;
      ( * ( QDataStream* ) this ) << message.getTime().getDate();
      ( * ( QDataStream* ) this ) << message.getTime().getTime();

      return *this;
   }

   /**
    * This function decodes a msg::Message object from binary data.
    * Just as the encoding operator, this also relies on the
    * QDataStream base class to do the heavy work. I assume that
    * this implementation should be quite robust, but haven't
    * actually tested it with corrupt binary data...
    *
    * @param message The Message object to read from the QIODevice
    * @returns This same object
    */
   BinaryStream& BinaryStream::operator>> ( Message& message ) {

      //
      // Temporary objects:
      //
      QString sender;
      QString text;
      quint16 level_num;
      QDate   date;
      QTime   time;

      //
      // Decode the msg::Message parameters in a specific order:
      //
      setVersion( QDataStream::Qt_4_4 );
      ( * ( QDataStream* ) this ) >> sender;
      ( * ( QDataStream* ) this ) >> text;
      ( * ( QDataStream* ) this ) >> level_num;
      ( * ( QDataStream* ) this ) >> date;
      ( * ( QDataStream* ) this ) >> time;

      //
      // Decode the message level:
      //
      std::map< quint16, Level >::const_iterator level_it;
      Level level;
      if( ( level_it = m_reverseMap.find( level_num ) ) ==
          m_reverseMap.end() ) {
         // This is a bad level!
         level = INFO;
      } else {
         level = level_it->second;
      }

      message.setSender( sender );
      message.setText( text );
      message.setLevel( level );
      message.setTime( MsgTime( date, time ) );

      return *this;
   }

   /**
    * Since the class has multiple constructors, it saves space
    * to have this initialisation in a separate function.
    */
   void BinaryStream::initMaps() {

      m_forwardMap[ VERBOSE ] = 1;
      m_forwardMap[ DEBUG ]   = 2;
      m_forwardMap[ INFO ]    = 3;
      m_forwardMap[ WARNING ] = 4;
      m_forwardMap[ ERROR ]   = 5;
      m_forwardMap[ FATAL ]   = 6;
      m_forwardMap[ ALWAYS ]  = 7;

      m_reverseMap[ 1 ] = VERBOSE;
      m_reverseMap[ 2 ] = DEBUG;
      m_reverseMap[ 3 ] = INFO;
      m_reverseMap[ 4 ] = WARNING;
      m_reverseMap[ 5 ] = ERROR;
      m_reverseMap[ 6 ] = FATAL;
      m_reverseMap[ 7 ] = ALWAYS;

      return;
   }

} // namespace msg
