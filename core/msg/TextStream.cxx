
// Qt include(s):
#include <QString>
#include <QStringList>

// Local include(s):
#include "TextStream.h"

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QStringList );

   /// Maximum printed length of a sender's name
   static const int MAXIMUM_SENDER_LENGTH = 20;

   /**
    * This constructor is useful for handling any kind of Qt devices
    * including files. Notice however that msg::TextStream can not be
    * used instead of a msg::BinaryStream to transmit information over
    * network sockets, since it doesn't provide decoding for its own
    * output.
    *
    * @param device Any kind of object inheriting from (and implementing) QIODevice
    */
   TextStream::TextStream( QIODevice* device )
      : QTextStream( device ) {

      initMap();
   }

   /**
    * This constructor can be used to handle the system file descriptors
    * such as <code>stdout</code> and <code>stderr</code>. Mostly
    * useful for writing messages to the console.
    *
    * @param handle A standard C file descriptor
    */
   TextStream::TextStream( FILE* handle )
      : QTextStream( handle, QIODevice::WriteOnly ) {

      initMap();
   }

   /**
    * This constructor should be useful for displaying messages in a
    * graphical environment.
    *
    * @param string A QString that should be filled
    */
   TextStream::TextStream( QString* string )
      : QTextStream( string, QIODevice::WriteOnly ) {

      initMap();
   }

   /**
    * I didn't really use this constructor for anything, but since
    * QTextStream provides such an interface, it seemed like a good
    * idea to have it in this class as well.
    *
    * @param array A QByteArray object where you want to write
    */
   TextStream::TextStream( QByteArray* array )
      : QTextStream( array, QIODevice::WriteOnly ) {

      initMap();
   }

   /**
    * This is the main function of this class. It formats the incoming
    * message in a user-friendly format.
    *
    * Note that there is not pair to this operator. The reading of messages
    * from their textual representation is not supported.
    *
    * @param message The message object that should be made "readable"
    * @returns This same object
    */
   TextStream& TextStream::operator<<( const Message& message ) {

      //
      // Construct the time string:
      //
      QString time = message.getTime().getDate().toString( Qt::SystemLocaleShortDate ) +
         "/" + message.getTime().getTime().toString( Qt::SystemLocaleShortDate );

      //
      // Format the sender's name:
      //
      QString sender;
      if( message.getSender().length() < MAXIMUM_SENDER_LENGTH ) {
         sender = message.getSender().leftJustified( MAXIMUM_SENDER_LENGTH,
                                                     ' ', true );
      } else {
         sender = message.getSender().leftJustified( MAXIMUM_SENDER_LENGTH - 3,
                                                     ' ', true );
         sender += "...";
      }

      //
      // Find the level string:
      //
      QString level;
      std::map< Level, QString >::const_iterator level_it;
      if( ( level_it = m_levelMap.find( message.getLevel() ) ) ==
          m_levelMap.end() ) {
         // This is a bad level!
         level = m_levelMap[ INFO ];
      } else {
         level = level_it->second;
      }

      //
      // Break up multi-line messages:
      //
      QStringList texts = message.getText().split( '\n' );

      //
      // Write each line separately (don't add newline at the end
      // of the last line):
      //
      for( QStringList::const_iterator text = texts.begin();
           text != texts.end(); ++text ) {

         ( * ( QTextStream* ) this ) << time << " " << sender << " " << level
                                     << " " << *text
                                     << ( ( text + 1 == texts.end() ) ?
                                          "" : "\n" );

      }

      return *this;
   }

   /**
    * msg::TextStream is not supposed to provide all the functionalities of
    * QTextStream, but this operator seemed to be quite important to have here
    * as well...
    *
    * @param text A QString that should be appended to the output
    * @returns This same object
    */
   TextStream& TextStream::operator<<( const QString& text ) {

      QTextStream::operator<<( text );
      return *this;
   }

   /**
    * Since the class has multiple constructors, it saves space
    * to have this initialisation in a separate function.
    */
   void TextStream::initMap() {

      m_levelMap[ VERBOSE ] = "VERBOSE";
      m_levelMap[ DEBUG ]   = "DEBUG  ";
      m_levelMap[ INFO ]    = "INFO   ";
      m_levelMap[ WARNING ] = "WARNING";
      m_levelMap[ ERROR ]   = "ERROR  ";
      m_levelMap[ FATAL ]   = "FATAL  ";
      m_levelMap[ ALWAYS ]  = "ALWAYS ";

      return;
   }

} // namespace msg
