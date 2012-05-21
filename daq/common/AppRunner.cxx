// $Id$

// Qt include(s):
#include <QtCore/QStringList>
#include <QtCore/QProcessEnvironment>

// Local include(s):
#include "AppRunner.h"

namespace daq {

   AppRunner::AppRunner( const QString& execName, const QString& options )
      : m_execName( execName ), m_options( options ),
        m_logger( "daq::AppRunner" ) {

   }

   /**
    * The destructor makes sure that the process is stopped before the object
    * is deleted.
    */
   AppRunner::~AppRunner() {

      if( m_process.state() == QProcess::Running ) stop();
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

      return ( m_process.state() == QProcess::Running );
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
      if( m_process.state() == QProcess::Running ) {
         m_logger << msg::WARNING << tr( "Trying to start a running process" )
                  << msg::endmsg;
         return false;
      } else {

         // Split the options at whitespaces. This is needed for using "execve":
         QStringList optionList = m_options.split( " ",
                                                   QString::SkipEmptyParts );
         m_process.setProcessEnvironment( QProcessEnvironment::systemEnvironment() );

         m_process.start( m_execName, optionList );
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
      if( m_process.state() == QProcess::Running ) {
         m_process.terminate();
         return true;
      } else {
         m_logger << msg::WARNING
                  << tr( "Stop requested for a stopped process" )
                  << msg::endmsg;
      }

      return false;
   }

} // namespace daq
