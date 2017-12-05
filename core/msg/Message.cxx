
// Local include(s):
#include "Message.h"

namespace msg {

   /**
    * The constructor receives the parameters in an order of
    * importance. So it is possible to only specify as many as
    * is really needed.
    *
    * @param sender The name of the component sending the message
    * @param text The actual message text
    * @param level The "level" of the message
    * @param time The time the message was sent
    */
   Message::Message( const QString& sender, const QString& text,
                     Level level, MsgTime time )
      : m_sender( sender ), m_text( text ),
        m_level( level ), m_time( time ) {

   }

   /**
    * @param sender The name of the component sending the message
    */
   void Message::setSender( const QString& sender ) {

      m_sender = sender;
      return;
   }

   /**
    * @param text The actual message text
    */
   void Message::setText( const QString& text ) {

      m_text = text;
      return;
   }

   /**
    * @param level The "level" of the message
    */
   void Message::setLevel( Level level ) {

      m_level = level;
      return;
   }

   /**
    * @param time The time the message was sent
    */
   void Message::setTime( MsgTime time ) {

      m_time = time;
      return;
   }

   /**
    * @returns The name of the component sending the message
    */
   const QString& Message::getSender() const {

      return m_sender;
   }

   /**
    * @returns The actual message text
    */
   const QString& Message::getText() const {

      return m_text;
   }

   /**
    * @returns The "level" of the message
    */
   Level Message::getLevel() const {

      return m_level;
   }

   /**
    * @returns The time the message was sent
    */
   const MsgTime& Message::getTime() const {

      return m_time;
   }

} // namespace msg
