//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_CORE_CERNLIB_NTUPLEMGR_H
#define CDA_CORE_CERNLIB_NTUPLEMGR_H

// CDA include(s):
#include "../common/StatusCode.h"
#include "../msg/Logger.h"

// Qt include(s):
#include <QCoreApplication>
#include <QString>

// STL include(s):
#include <vector>

/**
 *  @short Namespace for the CERNLIB interface classes
 *
 *         It turns out to be a good idea to use CERNLIB through C++
 *         interfaces. This way the CDA code only has a direct dependence
 *         on CERNLIB through these interfaces.
 *
 *         The interfaces were mainly created to be able to use CERNLIB
 *         from dynamic Qt plugins. Making a dynamic plugin depend on a
 *         static CERNLIB library just didn't work...
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
namespace cernlib {

/**
 *  @short Interface used to write HBOOK ntuples
 *
 *         This interface can be used to create and fill an HBOOK ntuple
 *         in an output file using CERNLIB. It provides functions for
 *         defining the ntuple contents, creating the ntuple, and adding
 *         new events to the ntuple.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class NTupleMgr {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(cernlib::NTupleMgr)

public:
   /// Constructor
   NTupleMgr();
   /// Destructor
   ~NTupleMgr();

   /// Add a new variable to the ntuple
   int addVar(const QString& name);
   /// Open an HBOOK file
   StatusCode openFile(const QString& fileName);
   /// Close the currently open HBOOK file
   void closeFile();

   /// Set the value of a variable in the current event
   StatusCode setVar(int index, float value);
   /// Add the event to the ntuple
   StatusCode saveEvent();

   static const int HFILE_ID;   ///< ID of the HBOOK file
   static const int NTUPLE_ID;  ///< ID of the ntuple

private:
   bool m_fileOpen;  ///< Status of the output file

   std::vector<QString> m_varNames;  ///< NTuple variable names
   std::vector<float> m_variables;   ///< NTuple event buffer

   int m_events;  ///< Number of events written to the ntuple

   msg::Logger m_logger;  ///< Message logging object

};  // class NTupleMgr

}  // namespace cernlib

#endif  // CDA_CORE_CERNLIB_NTUPLEMGR_H
