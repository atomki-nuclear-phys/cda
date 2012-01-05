// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_ROOT_NTUPLEMGR_H
#define CDA_CORE_ROOT_NTUPLEMGR_H

// STL include(s):
#include <list>

// Qt include(s):
#include <QtCore/QString>
#include <QtCore/QCoreApplication>

// Local include(s):
#include "../msg/Logger.h"

// Forward declaration(s):
#ifdef HAVE_ROOT_LIBS
class TFile;
class TTree;
#endif // HAVE_ROOT_LIBS

namespace root {

   /**
    *  @short Interface for creating ROOT ntuples (trees) from CDA
    *
    *         Just like cernlib::NTupleMgr, this object is supposed to provide
    *         an interface for the CDA devices for writing their data into
    *         ROOT ntuples (TTrees). It provides a much more flexible way
    *         of storing information than cernlib::NTupleMgr does.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class NTupleMgr {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( root::NTupleMgr )

   public:
      /// Default constructor
      NTupleMgr();
      /// Destructor
      ~NTupleMgr();

      /// Open a new ROOT file
      bool openFile( const QString& fileName, const QString& treeName = "cda" );
      /// Close the currently open file
      bool closeFile();

      /// Save the current event to the file
      bool saveEvent();

      /// Define a new variable for the ntuple
      template< typename TYPE >
      bool addVar( TYPE& obj, const QString& name, const QString& title = "" );

   private:
      /// Translate a typeid primitive type to something ROOT understands
      const char* rootType( const char* typeid_type ) const;
      /// Check that it is realistic to create the branch
      bool checkBranch( const char* name ) const;
      /// Create a primitive branch into the output TTree
      bool makeBranch( const char* name, void* address, const char* leaflist,
                       const char* title );
      /// Create an object branch into the output TTree
      bool makeBronch( const char* name, const char* classname, void* ptr,
                       const char* title );

      bool m_fileOpen; ///< Flag telling whether a ROOT file is open at the moment

#ifdef HAVE_ROOT_LIBS
      ::TFile* m_file; ///< Pointer to the current ROOT file
      ::TTree* m_tree; ///< Pointer to the TTree in the ROOT file
#endif // HAVE_ROOT_LIBS

      // We have to keep the pointers to the output variables defined by the user.
      // ROOT keeps track of the objects by storing pointers to pointers to the
      // objects. Since the user probably wants to use the output objects directly
      // and not through pointers, the base class has to take care of this
      // pointer issue by itself...
      std::list< void* > m_outputVarPointers;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class NTupleMgr

} // namespace root

// Include the template implementation:
#include "NTupleMgr.icc"

#endif // CDA_CORE_ROOT_NTUPLEMGR_H
