// $Id$

// Local include(s):
#include "MsgTime.h"

namespace msg {

   /**
    * This constructor can be used to create an object that stores the
    * time of its creation. Especially useful for tagging messages.
    */
   MsgTime::MsgTime()
      : m_date( QDate::currentDate() ), m_time( QTime::currentTime() ) {

   }

   /**
    * This is the most generic version of the constructor. It lets the user
    * initialise the object to any time.
    *
    * @param date The day the message was sent
    * @param time The time of the day the message was sent
    */
   MsgTime::MsgTime( const QDate& date, const QTime& time )
      : m_date( date ), m_time( time ) {

   }

   /**
    * This is a convinience constructor for initialising the object to the
    * specified time of today.
    *
    * @param hour The hour the message was sent
    * @param minute The minute the message was sent
    * @param second The second the message was sent
    * @param milisecond The mili-second the message was sent
    */
   MsgTime::MsgTime( int hour, int minute, int second, int milisecond )
      : m_date( QDate::currentDate() ), m_time( hour, minute, second,
                                                milisecond ) {

   }

   /**
    * This is a convinience constructor for initialising the object to
    * a given time.
    *
    * @param year The year the message was sent
    * @param month The month the message was sent
    * @param day The day the message was sent
    * @param hour The hour the message was sent
    * @param minute The minute the message was sent
    * @param second The second the message was sent
    * @param milisecond The mili-second the message was sent
    */
   MsgTime::MsgTime( int year, int month, int day, int hour, int minute,
                     int second, int milisecond )
      : m_date( year, month, day ), m_time( hour, minute, second,
                                            milisecond ) {

   }

   /**
    * This constructor is only here to make sure that the object contents
    * are properly copied. (Is probably not needed.)
    *
    * @param time The object to be copied
    */
   MsgTime::MsgTime( const MsgTime& time )
      : m_date( time.m_date ), m_time( time.m_time ) {

   }

   /**
    * @returns The time of the day the message was sent
    */
   const QTime& MsgTime::getTime() const {

      return m_time;
   }

   /**
    * @returns The day the message was sent
    */
   const QDate& MsgTime::getDate() const {

      return m_date;
   }

   /**
    * This operator could be needed in some complicated situations. Note,
    * that this operator provides a small possibility for changing the
    * stored time in an existing object. So it should be used carefully.
    *
    * @param time The object to be copied
    * @returns This same object
    */
   MsgTime& MsgTime::operator=( const MsgTime& time ) {

      m_date = time.m_date;
      m_time = time.m_time;

      return *this;
   }

} // namespace msg
