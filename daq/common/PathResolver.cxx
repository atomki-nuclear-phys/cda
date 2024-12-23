
// System include(s):
#include <vector>

// Qt include(s):
#include <QCoreApplication>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QStringList>

// CDA include(s):
#include "common/QStringCompat.h"

// Local include(s):
#include "PathResolver.h"

namespace daq {

//
// Initialise the member variables:
//
std::map<QString, std::list<QString> > PathResolver::m_environment{};
msg::Logger PathResolver::m_logger("daq::PathResolver");

/**
 * This is the main function of this class. It reads in the value of the
 * environment variable specified, and searches in the specified directories
 * for the given file name.
 *
 * The file name should not contain any slashes. If it does, the code
 * assumes that it's already a full path name and returns it unmodified.
 *
 * @returns The full path name of the file if successful, "" if the file
 *          could not be found, and the specified name itself if it
 *          contains any slashes
 */
QString PathResolver::resolve(const QString& name, const QString& env) {

   //
   // Access the cached environment variables. If it's not available
   // yet, then do the caching:
   //
   std::map<QString, std::list<QString> >::const_iterator envlist;
   if ((envlist = m_environment.find(env)) != m_environment.end()) {
      REPORT_VERBOSE(tr("Environment variable \"%1\" already cached").arg(env));
   } else {
      m_logger << msg::DEBUG << tr("Caching environment: %1").arg(env)
               << msg::endmsg;

      // For PATH, always consider first the directory that the current
      // application is in.
      if (env == "PATH") {
         m_environment[env].push_back(QCoreApplication::applicationDirPath());
         m_environment[env].push_back(QCoreApplication::applicationDirPath() +
                                      "/../../../");
      }

      // Get the value of the environment variable.
      const QProcessEnvironment procenv =
          QProcessEnvironment::systemEnvironment();
      const QString envarray = procenv.value(env);
      REPORT_VERBOSE(tr("%1 = %2").arg(env).arg(envarray));
#ifdef Q_OS_WIN
      static const QString separator = ";";
#else
      static const QString separator = ":";
#endif  // QT_ARCH_WINDOW
      QStringList envsplit = envarray.split(separator, Qt::SkipEmptyParts);
      for (QStringList::const_iterator element = envsplit.begin();
           element != envsplit.end(); ++element) {
         REPORT_VERBOSE(
             tr("Adding \"%1\" to environment %2").arg(*element).arg(env));
         m_environment[env].push_back(*element);
      }
      envlist = m_environment.find(env);
   }

   //
   // We don't look for files in subdirectories:
   //
   if (name.contains("/")) {
      m_logger << msg::WARNING
               << tr("The specified file name (%1) contains a slash.").arg(name)
               << std::endl
               << tr("Treating it as an absolute path...") << msg::endmsg;
      return name;
   }

   //
   // Loop over all the specified directories and see if they contain the
   // file that we're looking for:
   //
   for (std::list<QString>::const_iterator dir = envlist->second.begin();
        dir != envlist->second.end(); ++dir) {

      // Check if the file exists:
      QFileInfo finfo(*dir + "/" + name);
      if (finfo.exists()) {
         m_logger << msg::DEBUG
                  << tr("\"%1\" found under \"%2\"").arg(name).arg(*dir)
                  << msg::endmsg;
         return finfo.filePath();
      }
   }

   //
   // If we reached this point then we failed to find the file:
   //
   m_logger << msg::WARNING
            << tr("Failed to find \"%1\" in environment: %2").arg(name).arg(env)
            << msg::endmsg;
   return name;
}

}  // namespace daq
