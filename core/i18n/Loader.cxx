
// Qt include(s):
#include <QDir>
#include <QFileInfo>
#include <QProcessEnvironment>
#include <QStringList>
#include <QTranslator>

// Local include(s):
#include "Loader.h"

#ifndef Q_OS_WIN
// Found these definitions with Google. They make it possible
// to pass the CDASYS variable as a string to the code on all
// platforms.
#define CDASYS1_(x) #x
#define CDASYS_(x) CDASYS1_(x)
#define CDASYS_PATH CDASYS_(CDASYS)
#else
#include <QtCore/QCoreApplication>
#define CDASYS_PATH QCoreApplication::applicationDirPath() + "/.."
#endif  // CDASYS

namespace i18n {

/// Flag specifying if the hungarian translations should be loaded or not
static const bool USE_HUNGARIAN_TRANSLATIONS = false;

// Make sure that the Qt classes are in scope:
using QT_PREPEND_NAMESPACE(QDir);
using QT_PREPEND_NAMESPACE(QStringList);
using QT_PREPEND_NAMESPACE(QFileInfo);

Loader::Loader(const QString& path) : m_path(path), m_logger("i18n::Loader") {

   //
   // Check if a directory name has been specified:
   //
   if (m_path.isEmpty()) {

      // Get the process environment in a Qt way:
      const QProcessEnvironment env = QProcessEnvironment::systemEnvironment();

      // Look for CDASYS in the environment:
      if (env.contains("CDASYS")) {
         m_path = env.value("CDASYS");
      } else {
         // In case CDASYS is not in the environment, try using the
         // directory where the code was compiled:
         m_path = CDASYS_PATH;
      }
      m_path.append("/trans");

      m_logger << msg::DEBUG
               << tr("Setting translation directory to: %1").arg(m_path)
               << msg::endmsg;
   }
}

Loader::~Loader() {

   std::vector<QTranslator*>::iterator itr = m_translators.begin();
   std::vector<QTranslator*>::iterator end = m_translators.end();
   for (; itr != end; ++itr) {
      qApp->removeTranslator(*itr);
      delete *itr;
   }
   m_translators.clear();
}

bool Loader::loadTranslations() {

   // Return right away if we don't want to use the translations:
   if (!USE_HUNGARIAN_TRANSLATIONS)
      return true;

   //
   // Get the names of all physical files in the specified directory:
   //
   QDir pluginDir(m_path);
   QStringList files =
       pluginDir.entryList(QDir::Files | QDir::NoSymLinks | QDir::Readable);

   //
   // Loop over all file names:
   //
   for (QStringList::const_iterator pname = files.begin(); pname != files.end();
        ++pname) {

      if (!pname->contains("_hu.qm"))
         continue;

      QFileInfo finfo(*pname);
      QTranslator* trans = new QTranslator();
      if (!trans->load(finfo.fileName(), m_path)) {
         REPORT_ERROR(tr("Couldn't load translation file: %1/%2")
                          .arg(m_path)
                          .arg(finfo.fileName()));
         return false;
      }
      REPORT_VERBOSE(tr("Loaded translation file: %1").arg(finfo.fileName()));

      qApp->installTranslator(trans);
      m_translators.push_back(trans);
   }

   m_logger << msg::DEBUG << tr("Loaded all translations") << msg::endmsg;
   return true;
}

}  // namespace i18n
