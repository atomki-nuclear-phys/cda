// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_CERNLIB_NTUPLEMGR_H
#define CDA_CORE_CERNLIB_NTUPLEMGR_H

// STL include(s):
#include <vector>

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

// CDA include(s):
#include "../msg/Logger.h"
#include "../common/Export.h"

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
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
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
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT NTupleMgr {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( cernlib::NTupleMgr )

   public:
      /// Constructor
      NTupleMgr();
      /// Destructor
      ~NTupleMgr();

      /// Add a new variable to the ntuple
      int addVar( const QString& name );
      /// Open an HBOOK file
      bool openFile( const QString& fileName );
      /// Close the currently open HBOOK file
      void closeFile();

      /// Set the value of a variable in the current event
      bool setVar( int index, float value );
      /// Add the event to the ntuple
      void saveEvent();

      /// Clear all settings of the object
      void clear();

      static const int HFILE_ID;  ///< ID of the HBOOK file
      static const int NTUPLE_ID; ///< ID of the ntuple

   private:
      bool m_fileOpen; ///< Status of the output file

      std::vector< QString > m_varNames;  ///< NTuple variable names
      float*                 m_variables; ///< NTuple event buffer

      int m_events; ///< Number of events written to the ntuple

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class NTupleMgr

} // namespace cernlib

#endif // CDA_CORE_CERNLIB_NTUPLEMGR_H
