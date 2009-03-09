// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_COMMON_APPRUNNER_H
#define CDA_DAQ_COMMON_APPRUNNER_H

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

namespace daq {

   /**
    *  @short Class for starting various executables
    *
    *         The DAQ system consists of multiple processes running in parallel.
    *         This class can be used to create a new process, running a given
    *         executable.
    *
    *         One such object can be used to manage one process. So if you need
    *         to start the same executable in multiple instances, you'll have
    *         to create multiple instances of this class.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class AppRunner {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( daq::AppRunner )

   public:
      /// Default constructor
      AppRunner( const QString& execName = "", const QString& options = "" );
      /// Destructor
      ~AppRunner();

      /// Set the path to the executable
      void setExecName( const QString& execName );
      /// Get the path to the executable
      const QString& getExecName() const;

      /// Set the command line options of the executable
      void setOptions( const QString& options );
      /// Get the command line options of the executable
      const QString& getOptions() const;

      /// Queries whether the process is running
      bool isRunning() const;

      /// Start the execution of the process
      bool start();
      /// Stop the execution of the process
      bool stop();

   private:
      QString m_execName; ///< Path to the executable
      QString m_options; ///< Command line options

      int m_pid; ///< Process ID of the running executable
      bool m_running; ///< Flag showing if the process is running

      mutable msg::Logger m_logger; ///< Logger for the class

   }; // class AppRunner

} // namespace daq

#endif // CDA_DAQ_COMMON_APPRUNNER_H
