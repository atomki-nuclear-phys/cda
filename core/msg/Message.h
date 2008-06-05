// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_MESSAGE_H
#define CDA_CORE_MSG_MESSAGE_H

// Qt include(s):
#include <QtCore/QString>

// Local include(s):
#include "Level.h"
#include "MsgTime.h"

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "MSG::Message needs Qt >= 4.0.0"
#endif

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // MSG namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Class used for passing messages internally
    *
    *         This class should only be used by the message handling
    *         classes to pass messages around. The external users
    *         should not care about it too much.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Message {

   public:
      /// The only constructor
      Message( const QString& sender = "", const QString& text = "",
               Level level = INFO, MsgTime time = MsgTime() );

      /// Specify the sender of the message
      void setSender( const QString& sender );
      /// Specify the text of the message
      void setText( const QString& text );
      /// Specify the text of the message
      void setLevel( Level level );
      /// Specify the time of the message
      void setTime( MsgTime time );

      /// Get the sender of the message
      const QString& getSender() const;
      /// Get the text of the message
      const QString& getText()   const;
      /// Get the level of the message
      Level          getLevel()  const;
      /// Get the time of the message
      const MsgTime& getTime()   const;

   private:
      QString m_sender; ///< The sender of the message
      QString m_text;   ///< The text of the message
      Level   m_level;  ///< The level of the message
      MsgTime m_time;   ///< The time of the message

   }; // class Message

} // namespace msg

#endif // CDA_CORE_MSG_MESSAGE_H
