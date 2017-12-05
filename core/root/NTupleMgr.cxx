
// ROOT include(s):
#ifdef HAVE_ROOT_LIBS
#   include "TFile.h"
#   include "TTree.h"
#   include "TBranch.h"
#endif // HAVE_ROOT_LIBS

// Local include(s):
#include "NTupleMgr.h"

namespace root {

   NTupleMgr::NTupleMgr()
      : m_fileOpen( false ),
#ifdef HAVE_ROOT_LIBS
        m_file( 0 ), m_tree( 0 ),
#endif // HAVE_ROOT_LIBS
        m_outputVarPointers(),
        m_logger( "root::NTupleMgr" ) {

   }

   NTupleMgr::~NTupleMgr() {

      closeFile();
   }

   /**
    * This function has to be called during initialization to open a new output
    * ROOT file, and create a TTree inside of it for the data collected by CDA.
    *
    * @param fileName The name of the output file
    * @param treeName The name of the TTree in the output file
    * @returns <code>true</code> if the operation was successful, or
    *          <code>false</code> if it wasn't
    */
   bool NTupleMgr::openFile( const QString& fileName, const QString& treeName ) {

      // Tell the user what we're doing:
      m_logger << msg::DEBUG
               << tr( "Opening output file with name \"%1\" and tree name \"%2\"" )
         .arg( fileName ).arg( treeName )
               << msg::endmsg;

      // Check if an output file is already open:
      if( m_fileOpen ) {
         m_logger << msg::WARNING
                  << tr( "An output file is already open. Not doing anything." )
                  << msg::endmsg;
         return true;
      }

#ifdef HAVE_ROOT_LIBS
      // Open the output file:
      m_file = ::TFile::Open( fileName.toLatin1().constData(), "RECREATE" );
      if( ! m_file ) {
         REPORT_ERROR( tr( "Failed creating output file with name: %1" )
                       .arg( fileName ) );
         return false;
      } else {
         m_logger << msg::INFO << tr( "\"%1\" opened" ).arg( fileName )
                  << msg::endmsg;
      }
      m_file->cd();

      // Create the output TTree:
      m_tree = new ::TTree( treeName.toLatin1().constData(),
                            "Data collected by CDA" );
      m_tree->SetAutoSave( 100000000 ); // Never auto-save
      m_tree->SetAutoFlush( 30000000 ); // Auto-flush after 30 MB was collected
      TTree::SetBranchStyle( 1 );
      m_tree->SetDirectory( m_file );
#endif // HAVE_ROOT_LIBS

      m_fileOpen = true;
      return true;
   }

   /**
    * At the end of data taking this function has to be used to close the output
    * file cleanly.
    *
    * @returns <code>true</code> if the operation was successful, or
    *          <code>false</code> if it wasn't
    */
   bool NTupleMgr::closeFile() {

      // Check if the function needs to do anything:
      if( ! m_fileOpen ) {
         m_logger << msg::DEBUG
                  << tr( "No output file is open at the moment" )
                  << msg::endmsg;
         return true;
      }

      // Number of events written out:
      long long int events = 0;

#ifdef HAVE_ROOT_LIBS
      // A security check:
      if( ( ! m_file ) || ( ! m_tree ) ) {
         REPORT_ERROR( tr( "The object is in an inconsistent state" ) );
         return false;
      }

      // Write out the last of the TTree:
      if( m_tree->Write() <= 0 ) {
         REPORT_ERROR( tr( "An error happened while writing out the final events" ) );
         return false;
      }
      events = m_tree->GetEntries(); // Remember the number of events written out
      m_tree->SetDirectory( 0 );
      delete m_tree;
      m_tree = 0;

      // Now close the output file:
      m_file->Close();
      delete m_file;
      m_file = 0;

      // Clear the variable pointer cache:
      m_outputVarPointers.clear();
#endif // HAVE_ROOT_LIBS

      // Tell the user what we've done:
      m_logger << msg::INFO
               << tr( "Closed output file after writing %1 events" )
         .arg( events )
               << msg::endmsg;

      m_fileOpen = false;
      return true;
   }

   /**
    * This function should be called after every event, to save it into the
    * output file.
    *
    * @returns <code>true</code> if the operation was successful, or
    *          <code>false</code> if it wasn't
    */
   bool NTupleMgr::saveEvent() {

      // Check if a file has been opened:
      if( ! m_fileOpen ) {
         REPORT_ERROR( tr( "A file is not open at the moment" ) );
         return false;
      }

#ifdef HAVE_ROOT_LIBS
      // A security check:
      if( ( ! m_file ) || ( ! m_tree ) ) {
         REPORT_ERROR( tr( "A TTree is not available at the moment" ) );
         return false;
      }

      // Fill the current event to the output file:
      if( m_tree->Fill() <= 0 ) {
         REPORT_ERROR( tr( "An error happened while writing the current event" ) );
         return false;
      }
#endif // HAVE_ROOT_LIBS

      return true;
   }

   const char* NTupleMgr::rootType( const char* typeid_type ) const {

      // Check that the parameter describes a primitive type:
      if( ::strlen( typeid_type ) != 1 ) {
         REPORT_ERROR( tr( "Received a complex object description" ) );
         return "";
      }

      // Translate the type name:
      switch( typeid_type[ 0 ] ) {

      case 'c':
         return "B";
         break;
      case 'h':
         return "b";
         break;
      case 's':
         return "S";
         break;
      case 't':
         return "s";
         break;
      case 'i':
         return "I";
         break;
      case 'j':
         return "i";
         break;
      case 'f':
         return "F";
         break;
      case 'd':
         return "D";
         break;
      case 'x':
         return "L";
         break;
      case 'y':
         return "l";
         break;
      case 'b':
         return "O";
         break;
      default:
         REPORT_ERROR( tr( "Unknown primitive type encountered: %1" )
                       .arg( typeid_type ) );
         return "";
         break;
      }

      REPORT_ERROR( tr( "Unknown primitive type encountered: %1" )
                    .arg( typeid_type ) );
      return "";
   }

   bool NTupleMgr::checkBranch( const char* name ) const {

#ifdef HAVE_ROOT_LIBS
      // Check that the output is already open:
      if( ( ! m_fileOpen ) || ( ! m_tree ) ) {
         REPORT_ERROR( tr( "You have to open an output file before declaring "
                           "variables" ) );
         return false;
      }

      // Check if the branch exists already:
      if( m_tree->GetBranch( name ) ) {
         REPORT_ERROR( tr( "A branch with name \"%1\" already exists" )
                       .arg( name ) );
         return false;
      }
#else
      REPORT_VERBOSE( tr( "Checking if branch with name \"%1\" can be added" )
                      .arg( name ) );
#endif // HAVE_ROOT_LIBS

      return true;
   }

   bool NTupleMgr::makeBranch( const char* name, void*
#ifdef HAVE_ROOT_LIBS
                                                       address
#endif // HAVE_ROOT_LIBS
                               , const char* leaflist, const char* title ) {

#ifdef HAVE_ROOT_LIBS
      // Create the branch:
      TBranch* br = m_tree->Branch( name, address, leaflist );
      if( ! br ) {
         REPORT_ERROR( tr( "Failed creating branch with name: %1" )
                       .arg( name ) );
         return false;
      }
      // Set its title:
      br->SetTitle( title );
#else
      REPORT_VERBOSE( tr( "Creating primitive branch with name \"%1\" , "
                          "title \"%2\" and leaf-list \"%3\"" ).arg( name )
                      .arg( title ).arg( leaflist ) );
#endif // HAVE_ROOT_LIBS

      return true;
   }

   bool NTupleMgr::makeBronch( const char* name, const char* classname,
                               void*
#ifdef HAVE_ROOT_LIBS
                                     ptr
#endif // HAVE_ROOT_LIBS
                               , const char* title ) {

#ifdef HAVE_ROOT_LIBS
      // Create the branch:
      TBranch* br = m_tree->Bronch( name, classname, ptr );
      if( ! br ) {
         REPORT_ERROR( tr( "Failed creating branch with name: %1" )
                       .arg( name ) );
         return false;
      }
      // Set its title:
      br->SetTitle( title );
#else
      REPORT_VERBOSE( tr( "Creating object branch with name \"%1\", title "
                          "\"%2\" and classname \"%3\"" ).arg( name )
                      .arg( title ).arg( classname ) );
#endif // HAVE_ROOT_LIBS

      return true;
   }

} // namespace root
