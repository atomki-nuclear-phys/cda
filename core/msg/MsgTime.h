// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_MSG_MSGTIME_H
#define CDA_CORE_MSG_MSGTIME_H

// Qt include(s):
#include <QDate>
#include <QTime>

// CDA include(s):
#include "../common/Export.h"

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QDate );
   using QT_PREPEND_NAMESPACE( QTime );

   /**
    *  @short Internal class for storing the time of a message
    *
    *         It is important to know when a message was sent.
    *         It can be used to look up when a certain problem
    *         (or calculated event) happened during the data
    *         acquisition.
    *
    *         I actually wanted to name the class Time, but naming
    *         the header file Time.h caused annoying problems on
    *         MacOS X (with the FS being case-insensitive...).
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDACORE_EXPORT MsgTime {

   public:
      /// Constructor initialising to current time
      MsgTime();
      /// Constructor using Qt objects
      MsgTime( const QDate& date, const QTime& time );
      /// Constructor initialising to specified time
      MsgTime( int hour, int minute, int second = 0, int milisecond = 0 );
      /// Constructor initialising to specified time
      MsgTime( int year, int month, int day, int hour, int minute,
               int second = 0, int milisecond = 0 );
      /// Copy constructor
      MsgTime( const MsgTime& time );

      /// Get the time of the day of the message
      const QTime& getTime() const;
      /// Get the day of the message
      const QDate& getDate() const;

      /// Assignment operator
      MsgTime& operator=( const MsgTime& time );

   private:
      QDate m_date; ///< The stored day
      QTime m_time; ///< The stored time of the day

   }; // class MsgTime

} // namespace msg

#endif // CDA_CORE_MSG_MSGTIME_H
