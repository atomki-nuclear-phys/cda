// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DAQ_COMMON_PATHRESOLVER_H
#define CDA_DAQ_COMMON_PATHRESOLVER_H

// STL include(s):
#include <list>
#include <map>

// Qt include(s):
#include <QCoreApplication>
#include <QString>

// CDA include(s):
#include "msg/Logger.h"

// Local include(s):
#include "Export.h"

namespace daq {

/**
 *  @short Helper class for finding the full path name of files
 *
 *         This helper class will mainly be used for finding the absolute
 *         path names of the CDA executables, but in general it could be
 *         used for other things as well. (Finding libraries, finding
 *         configuration files in specific locations. etc.)
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CDADAQ_EXPORT PathResolver {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(daq::PathResolver)

public:
   /// Find the full path of a given file
   static QString resolve(const QString& name, const QString& env = "PATH");

private:
   /// Cached environments
   static std::map<QString, std::list<QString> > m_environment;
   /// Logger for the class
   static msg::Logger m_logger;

};  // class PathResolver

}  // namespace daq

#endif  // CDA_DAQ_COMMON_PATHRESOLVER_H
