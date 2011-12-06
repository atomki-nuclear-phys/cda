// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// The include(s) needed on different platforms:
#ifdef Q_WS_WIN32
#   include <windows.h>
#else
extern "C" {
#   include <sys/time.h>
#   include <sys/types.h>
#   include <unistd.h>
}
#endif

// Local include(s):
#include "Sleep.h"

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

#ifdef Q_WS_WIN32
      // On Windows we can use this built-in function:
      ::Sleep( milisec );
#else
      // On POSIX systems the most accurate one is the
      // select(...) function:
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = milisec * 1000;
      select( 0, NULL, NULL, NULL, &tv );
#endif

      return;
   }

   /**
    * The VME readout relies on as short of a sleep cycle as possible on the
    * platform. This means ~1 miliseconds on Windows, and ~100 microseconds on
    * Linux/MacOS X.
    */
   void SleepMin() {

#ifdef Q_WS_WIN32
      // On Windows we can use this built-in function:
      ::Sleep( 1 );
#else
      // On POSIX systems the most accurate one is the
      // select(...) function:
      struct timeval tv;
      tv.tv_sec = 0;
      tv.tv_usec = 100;
      select( 0, NULL, NULL, NULL, &tv );
#endif

      return;
   }

}
