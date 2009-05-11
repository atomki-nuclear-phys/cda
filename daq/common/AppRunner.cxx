// $Id$

// System include(s):
#include <cstdlib>
extern "C" {
#   include <unistd.h>
#   include <sys/types.h>
#   include <signal.h>
}

#include <QtCore/QtGlobal>
#ifdef Q_OS_DARWIN
#include <crt_externs.h>
#define environ ( *_NSGetEnviron() )
#endif

// Qt include(s):
#include <QtCore/QStringList>

// Local include(s):
#include "AppRunner.h"

namespace daq {

   AppRunner::AppRunner( const QString& execName, const QString& options )
      : m_execName( execName ), m_options( options ), m_pid( 0 ),
        m_running( false ), m_logger( "daq::AppRunner" ) {

   }

   /**
    * The destructor makes sure that the process is stopped before the object
    * is deleted.
    */
   AppRunner::~AppRunner() {

      if( m_running ) stop();

   }

   void AppRunner::setExecName( const QString& execName ) {

      m_execName = execName;
      return;

   }

   const QString& AppRunner::getExecName() const {

      return m_execName;

   }

   void AppRunner::setOptions( const QString& options ) {

      m_options = options;
      return;

   }

   const QString& AppRunner::getOptions() const {

      return m_options;

   }

   bool AppRunner::isRunning() const {

      return m_running;

   }

   /**
    * This is the main function of the class. It constructs a classical array of
    * character arrays for the command line arguments, because the system calls
    * don't like C++ constructs...
    *
    * Once done with setting up all the input data, it starts the new process by
    * first duplicating the caller process (using <code>fork</code>), then starting
    * the specified executable inside the newly created process (using
    * <code>execve</code>).
    *
    * When finished, it tries to clean up the memory in the caller (original) process.
    *
    * @returns <code>true</code> if tha process started successfully,
    *          <code>false</code> is there were any problems
    */
   bool AppRunner::start() {

      //
      // See if we have to do anything:
      //
      if( m_running ) {
         m_logger << msg::WARNING << tr( "Trying to start a running process" )
                  << msg::endmsg;
         return false;
      } else {

         // Split the options at whitespaces. This is needed for using "execve":
         QStringList optionList = m_options.split( " ", QString::SkipEmptyParts );

         //
         // Construct a nice C array with the arguments. This needs some old-school
         // C memory management which I suck at. So the code might be sub-optimal...
         //
         // The first argument is the path to the executable itself. It's just how
         // POSIX systems like it...
         //
         char* argv[ optionList.size() + 2 ];
         argv[ 0 ] = reinterpret_cast< char* >( ::malloc( m_execName.size() ) );
         ::sprintf( argv[ 0 ], "%s", m_execName.toLatin1().constData() );
         for( int i = 1; i < optionList.size() + 1; ++i ) {
            argv[ i ] = reinterpret_cast< char* >( ::malloc( optionList.at( i - 1 ).size() ) );
            ::sprintf( argv[ i ], "%s", optionList.at( i - 1 ).toLatin1().constData() );
         }
         argv[ optionList.size() + 1 ] = '\0';

         //
         // Start the executable. The program is started with the arguments (options)
         // specified to this object, and receives the same environment that this code
         // runs in.
         //
         if( ( m_pid = ::fork() ) == 0 ) {
            if( ::execve( m_execName.toLatin1().constData(), argv, environ ) == -1 ) {
               m_logger << msg::ERROR << tr( "There was a problem starting program: %1" )
                  .arg( m_execName ) << msg::endmsg;
               exit( 255 );
               return false;
            }
         }

         //
         // Let's free up all the space allocated for the arguments:
         //
         for( int i = 0; i < optionList.size() + 1; ++i ) {
            ::free( argv[ i ] );
         }

         // Remember that the executable is now running:
         m_running = true;

      }

      return true;

   }

   /**
    * This function can stop a running process by sending it an interrupt signal.
    *
    * All executables in the CDA system should be programmed to handle interrupt
    * signals gracefully, closing all their open ports, and exiting clearly.
    *
    * @returns <code>true</code> if the process was successfully terminated,
    *          <code>false</code> if any problems were seen
    */
   bool AppRunner::stop() {

      //
      // Only do something if the process has already been started:
      //
      if( m_running ) {

         //
         // Send an interrupt signal to the process:
         //
         if( ! ::kill( m_pid, SIGINT ) ) {
            m_logger << msg::DEBUG << tr( "Killed process with ID: %1 (%2)" )
               .arg( m_pid ).arg( m_execName ) << msg::endmsg;
            m_pid = 0;
            m_running = false;
            return true;
         } else {
            m_logger << msg::ERROR << tr( "Failed to kill process with ID: %1" )
               .arg( m_pid ) << msg::endmsg;
            return false;
         }

      } else {
         m_logger << msg::WARNING << tr( "Stop requested for a stopped process" )
                  << msg::endmsg;
      }

      return false;

   }

} // namespace daq
