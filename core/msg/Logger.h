// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_LOGGER_H
#define CDA_CORE_MSG_LOGGER_H

// STL include(s):
#include <sstream>

// Qt include(s):
#include <QtCore/QString>

// Local include(s):
#include "Level.h"
#include "Sender.h"

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "MSG::Logger needs Qt >= 4.0.0"
#endif

/**
 *  @short Namespace for the message handling classes
 *
 *         All classes that have to do with message handling are put into
 *         this namespace. This way the names of the classes can be quite
 *         generic (Socket, Sender, etc.) which I prefer.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // MSG namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Main message logging class
    *
    *         This class should be used in all parts of the code to display
    *         messages. The class behaves like any std::ostream, so all
    *         the practices used with those can be used with this as well.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Logger : public std::ostringstream {

      friend Logger& endmsg( Logger& );

   public:
      /// Normal constructor that defines the sender of the messager
      Logger( const QString& sender = "" );
      /// Copy constructor
      Logger( const Logger& logger );

      /// Assignment operator
      Logger& operator=( const Logger& logger );

      /// Operator accepting Logger stream modifiers
      Logger& operator<<( Logger& ( *_f )( Logger& ) );
      /// Operator accepting std::ostream stream modifiers
      Logger& operator<<( std::ostream& ( *_f )( std::ostream& ) );
      /// Operator accepting std::ios stream modifiers
      Logger& operator<<( std::ios& ( *_f )( std::ios& ) );
      /// Operator accepting message level setting
      Logger& operator<<( Level level );
      /// Operator accepting QString-s:
      Logger& operator<<( const QString& msg );
      /// Operator accepting basically any kind of argument
      /**
       * MSG::Logger was designed to give all the features that std::ostream
       * objects usually provide. This operator handles all kinds of
       * arguments and passes it on to the std::ostringstream base class.
       */
      template < class T > Logger& operator<<( T arg ) {
         if( m_activeLevel >= m_msgSender->getMinLevel() ) {
            ( * ( std::ostringstream* ) this ) << arg;
         }
         return *this;
      }

   private:
      /// Internal function giving a message to MSG::Sender
      void send();

      QString m_sender;      ///< Name of the component sending the messages
      Level   m_activeLevel; ///< Level of the message to be sent

      Sender* m_msgSender;   ///< Pointer to the MSG::Sender singleton

   }; // class Logger

   ///////////////////////////////////////////////////////////////////
   //                                                               //
   //    The stream modifier absorbing functions are declared       //
   //    inline to speed up the code a bit.                         //
   //                                                               //
   ///////////////////////////////////////////////////////////////////

   inline Logger& Logger::operator<<( Logger& ( *_f )( Logger& ) ) {

      if( m_activeLevel >= m_msgSender->getMinLevel() ) {
         ( _f )( *this );
      }
      return *this;

   }

   inline Logger& Logger::operator<<( std::ostream& ( *_f )( std::ostream& ) ) {

      if( m_activeLevel >= m_msgSender->getMinLevel() ) {
         ( _f )( *this );
      }
      return *this;

   }

   inline Logger& Logger::operator<<( std::ios& ( *_f )( std::ios& ) ) {

      if( m_activeLevel >= m_msgSender->getMinLevel() ) {
         ( _f )( *this );
      }
      return *this;

   }

   /// Stream modifier that sends off a message to the server
   Logger& endmsg( Logger& logger );

} // namespace msg

#endif // CDA_CORE_MSG_LOGGER_H
