# $Id$
#
# - Finds ROOT instalation
# This module sets up ROOT information
# It defines:
# ROOT_FOUND          If the ROOT is found
# ROOT_INCLUDE_DIR    PATH to the include directory
# ROOT_INCLUDE_DIRS   PATH to the include directories (not cached)
# ROOT_LIBRARIES      Most common libraries
# ROOT_<name>_LIBRARY Full path to the library <name>
# ROOT_LIBRARY_DIR    PATH to the library directory
# ROOT_DEFINITIONS    Compiler definitions
# ROOT_CXX_FLAGS      Compiler flags to used by client packages
# ROOT_C_FLAGS        Compiler flags to used by client packages
#

# First try to find ROOT using the CMake code bundled with ROOT itself:
set( _pathBackup ${CMAKE_MODULE_PATH} )
set( CMAKE_MODULE_PATH )
find_package( ROOT QUIET )

# If it was found like that, yay!
if( ROOT_FOUND )
   message( STATUS "Found ROOT: ${ROOT_BINARY_DIR}/root-config "
                   "(found version \"${ROOT_VERSION}\")" )
   # Make use of the imported libraries:
   if( "${ROOT_VERSION}" VERSION_LESS 6.10 )
      set( ROOT_LIBRARIES Core RIO Net Hist Graf Graf3d Gpad Tree
                          Rint Postscript Matrix Physics MathCore Thread )
   else()
      set( ROOT_LIBRARIES ROOT::Core ROOT::RIO ROOT::Net ROOT::Hist ROOT::Graf
                          ROOT::Graf3d ROOT::Gpad ROOT::Tree ROOT::Rint
                          ROOT::Postscript ROOT::Matrix ROOT::Physics
                          ROOT::MathCore ROOT::Thread )
   endif()
else()

   # Look for root-config. Everything else is found using this script:
   find_program( ROOT_CONFIG_EXECUTABLE root-config
      HINTS $ENV{ROOTSYS}/bin )
   mark_as_advanced( ROOT_CONFIG_EXECUTABLE )

   # Get all the necessary variables:
   execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --prefix
      OUTPUT_VARIABLE ROOTSYS
      OUTPUT_STRIP_TRAILING_WHITESPACE )

   execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --version
      OUTPUT_VARIABLE ROOT_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE )

   execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --incdir
      OUTPUT_VARIABLE ROOT_INCLUDE_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE )
   set( ROOT_INCLUDE_DIRS ${ROOT_INCLUDE_DIR} )

   execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --libdir
      OUTPUT_VARIABLE ROOT_LIBRARY_DIR
      OUTPUT_STRIP_TRAILING_WHITESPACE )
   set( ROOT_LIBRARY_DIRS ${ROOT_LIBRARY_DIR} )

   # Find all the standard plus requested libraries:
   set( rootlibs Core Cint RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript
                 Matrix Physics MathCore Thread )
   set( ROOT_LIBRARIES )
   foreach( _cpt ${rootlibs} ${ROOT_FIND_COMPONENTS} )
      find_library( ROOT_${_cpt}_LIBRARY ${_cpt} HINTS ${ROOT_LIBRARY_DIR} )
      if( ROOT_${_cpt}_LIBRARY )
         mark_as_advanced( ROOT_${_cpt}_LIBRARY )
         list( APPEND ROOT_LIBRARIES ${ROOT_${_cpt}_LIBRARY} )
      endif()
   endforeach()
   if( ROOT_LIBRARIES )
      list( REMOVE_DUPLICATES ROOT_LIBRARIES )
   endif()

   # Construct the compiler flags:
   execute_process(
      COMMAND ${ROOT_CONFIG_EXECUTABLE} --cflags
      OUTPUT_VARIABLE __cflags
      OUTPUT_STRIP_TRAILING_WHITESPACE )
   string( REGEX MATCHALL "-(D|U)[^ ]*" ROOT_DEFINITIONS "${__cflags}" )
   string( REGEX REPLACE "(^|[ ]*)-I[^ ]*" "" ROOT_CXX_FLAGS "${__cflags}" )
   string( REGEX REPLACE "(^|[ ]*)-I[^ ]*" "" ROOT_C_FLAGS "${__cflags}" )

   # Handle the standard find_package arguments:
   include( FindPackageHandleStandardArgs )
   find_package_handle_standard_args( ROOT FOUND_VAR ROOT_FOUND
      REQUIRED_VARS ROOT_CONFIG_EXECUTABLE ROOTSYS ROOT_INCLUDE_DIR
                    ROOT_LIBRARIES ROOT_LIBRARY_DIR
      VERSION_VAR ROOT_VERSION )

endif()

# Restore the path:
set( CMAKE_MODULE_PATH ${_pathBackup} )
unset( _pathBackup )
