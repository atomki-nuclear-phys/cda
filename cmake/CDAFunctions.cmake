#
# Helper functions used for building the CDA source code.
#

# Include(s) needed by the module:
include( CMakeParseArguments )

# Function used to build the CDA libraries
#
# This function is used to build the main libraries of CDA in a consistent way.
#
# Usage: cda_add_library( cdacore
#                         HEADERS msg/*.h ...
#                         SOURCES msg/*.cxx ...
#                         INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ...
#                         LINK_LIBRARIES ${Qt45_LIBRARIES} ... )
#                         DEFINITIONS HAVE_ROOT_LIBS ... )
#
function( cda_add_library name )

   # Parse the function parameters:
   cmake_parse_arguments( ARG "" ""
      "HEADERS;SOURCES;INCLUDE_DIRS;LINK_LIBRARIES;DEFINITIONS" ${ARGN} )

   # Collect the header file names:
   set( _headers )
   foreach( _header ${ARG_HEADERS} )
      file( GLOB _files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${_header} )
      foreach( _file ${_files} )
         list( APPEND _headers ${_file} )
      endforeach()
      unset( _files )
   endforeach()

   # Collect the source file names:
   set( _sources )
   foreach( _source ${ARG_SOURCES} )
      file( GLOB _files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
         ${_source} )
      foreach( _file ${_files} )
         list( APPEND _sources ${_file} )
      endforeach()
      unset( _files )
   endforeach()

   # Group all source files:
   foreach( f ${_headers} ${_sources} )
      # Get the file's path:
      get_filename_component( _path ${f} PATH )
      # Replace the forward slashes with double backward slashes:
      string( REPLACE "/" "\\\\" _group "${_path}" )
      # Put the file into the right group:
      source_group( ${_group} FILES ${f} )
   endforeach()
   unset( _path )
   unset( _group )

   # Declare the library:
   add_library( ${name} STATIC ${_headers} ${_sources} )

   # Set up the include directories to use for its build:
   target_include_directories( ${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
      ${ARG_INCLUDE_DIRS} )

   # Set up the libraries to link to it:
   target_link_libraries( ${name} ${ARG_LINK_LIBRARIES} )

   # Set up the compile definitions for it:
   target_compile_definitions( ${name} PUBLIC ${ARG_DEFINITIONS} )

   # Definition used for building a shared library. In case we ever switch back
   # to building such things...
   string( TOUPPER ${name} _nameUpper )
   target_compile_definitions( ${name} PRIVATE "${_nameUpper}_LIBRARY" )

   # Put the library into a folder in an appropriate IDE (Xcode, VS, etc.):
   string( REPLACE "${CMAKE_SOURCE_DIR}/" "" _libraryFolder
      ${CMAKE_CURRENT_SOURCE_DIR} )
   set_target_properties( ${name} PROPERTIES FOLDER ${_libraryFolder} )

   # Clean up:
   unset( _headers )
   unset( _sources )
   unset( _nameUpper )
   unset( _libraryFolder )

endfunction( cda_add_library )
