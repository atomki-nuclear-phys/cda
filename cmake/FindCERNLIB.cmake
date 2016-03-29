# $Id$
#
# Simple module for finding CERNLIB. Mainly as it is installed
# on Ubuntu systems.
#
# Sets the following variables:
#  - CERNLIB_INCLUDE_DIRS: Include directories to be used to build
#    code against the CERNLIB libraries.
#  - CERNLIB_LIBRARIES: Libraries to link to use the CERNLIB code.
#  - CERNLIB_LIBRARY_DIRS: Directories holding the CERNLIB libraries.
#

# Find the cernlib helper script:
find_program( CERNLIB_EXECUTABLE cernlib
  HINTS /cern/pro/bin /sw/bin )

# Find the include directories to use:
find_path( CERNLIB_INCLUDE_DIR NAMES cfortran/cfortran.h hbook.h
  HINTS /cern/pro/include /sw/include )
set( CERNLIB_INCLUDE_DIRS ${CERNLIB_INCLUDE_DIR} )
mark_as_advanced( CERNLIB_INCLUDE_DIR )

# Get the list of libraries to link against:
execute_process( COMMAND ${CERNLIB_EXECUTABLE} ${CERNLIB_FIND_COMPONENTS}
  OUTPUT_VARIABLE CERNLIB_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE )
mark_as_advanced( CERNLIB_LIBRARIES )

# Find the directory holding packlib. And assume that all libraries are
# in the same place.
find_library( _packlib NAME packlib
   HINTS /cern/pro/lib /sw/lib )
get_filename_component( CERNLIB_LIBRARY_DIR "${_packlib}" PATH )
set( CERNLIB_LIBRARY_DIRS ${CERNLIB_LIBRARY_DIR} )
unset( _packlib )
mark_as_advanced( _packlib )

# On MacOS add gfortran as a requirement by hand. Since the cernlib
# script doesn't seem to be aware of this dependency of the libraries. :-(
if( APPLE )
   find_library( _gfortran NAME gfortran
      HINTS /usr/local/gfortran/lib /sw/lib )
   list( APPEND CERNLIB_LIBRARIES ${_gfortran} )
   get_filename_component( _dir "${_gfortran}" PATH )
   list( APPEND CERNLIB_LIBRARY_DIRS ${_dir} )
   unset( _dir )
   unset( _gfortran )
   mark_as_advanced( _gfortran )
endif()

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CERNLIB DEFAULT_MSG
  CERNLIB_EXECUTABLE CERNLIB_INCLUDE_DIR CERNLIB_LIBRARIES )
mark_as_advanced( CERNLIB_FOUND )
