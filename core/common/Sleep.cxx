// $Id$

// Qt include(s):
#include <QtCore/QThread>

// Local include(s):
#include "Sleep.h"

namespace {

   /**
    *  @short Class exposing Qt's thread waiting functions
    *
    *         Qt has very reliable functions for sending a thread to sleep
    *         for a specified amount of time. Unfortunately QThread hides
    *         these functions from the outside world by default, but that's
    *         nothing that a little C++ magic can't fix.
    *
    *         Using the select(...) function in a multi-threaded environment
    *         on MacOS X turned out to be unreliable, so this way I'll just
    *         make use of the Qt team's knowledge of the platforms that Qt
    *         is supported on...
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class ThreadWait : public QThread {

   public:
      /// Sleep the current thread for a specified number of miliseconds
      static void msleep( unsigned long msecs ) {
         QThread::msleep( msecs );
         return;
      }

      /// Sleep the current thread for a specified number of microseconds
      static void usleep( unsigned long usecs ) {
         QThread::usleep( usecs );
         return;
      }

   }; // class ThreadWait

} // private namespace

namespace common {

   /**
    * Since the code needs to let various applications/threads sleep for
    * a little while in a few places, and the way to make the application
    * sleep is very different on POSIX and Windows systems, it seemed
    * the easiest to hide the implementation of making the process sleep
    * behind this function.
    *
    * @param milisec Miliseconds to sleep
    */
   void Sleep( unsigned int milisec ) {

      ThreadWait::msleep( milisec );
      return;
   }

   /**
    * The VME readout relies on as short of a sleep cycle as possible on the
    * platform. This means ~1 miliseconds on Windows, and ~100 microseconds on
    * Linux/MacOS X.
    */
   void SleepMin() {

      ThreadWait::usleep( 100 );
      return;
   }

}
