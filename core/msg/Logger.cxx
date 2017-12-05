
// Local include(s):
#include "Logger.h"
#include "Sender.h"
#include "Message.h"

namespace msg {

   /**
    * @param sender The name of the component sending the messages
    */
   Logger::Logger( const QString& sender )
      : std::ostringstream(), m_sender( sender ), m_activeLevel( INFO ) {

      m_msgSender = Sender::instance();
   }

   /**
    * @param logger The object that should be copied
    */
   Logger::Logger( const Logger& logger )
      : std::basic_ios< Logger::char_type, Logger::traits_type >(),
        std::ostringstream() {

      *this = logger;
   }

   /**
    * @param logger The object that should be copied
    * @returns This object
    */
   Logger& Logger::operator=( const Logger& logger ) {

      m_sender      = logger.m_sender;
      m_activeLevel = logger.m_activeLevel;
      m_msgSender   = logger.m_msgSender;

      return *this;
   }

   /**
    * @param level The level of the message to come
    * @returns This same object
    */
   Logger& Logger::operator<<( Level level ) {

      m_activeLevel = level;
      return *this;
   }

   /**
    * @param msg The message defined as a QString
    * @returns This same object
    */
   Logger& Logger::operator<<( const QString& msg ) {

      *this << msg.toLatin1().constData();
      return *this;
   }

   /**
    * The function constructs a msg::Message object from the data collected
    * since the last message sending, gives it to msg::Sender and then
    * resets its internal buffer.
    */
   void Logger::send() {

      //
      // Create the message and give it to the Sender singleton:
      //
      Message message( m_sender, this->str().c_str(), m_activeLevel );
      m_msgSender->send( message );

      //
      // Reset the stream buffer:
      //
      this->str( "" );

      return;
   }

   /**
    * This function works pretty much like std::endl. When you set up a message,
    * you can just call <code>logger << msg::endmsg</code> and the message
    * will be sent to the message server.
    *
    * @param logger The logger object to send the message from
    * @returns The logger object given to the function
    */
   Logger& endmsg( Logger& logger ) {

      logger.send();
      return logger;
   }

} // namespace msg
