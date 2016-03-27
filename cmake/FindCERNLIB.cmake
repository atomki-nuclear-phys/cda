# $Id$
#
# Simple module for finding CERNLIB. Mainly as it is installed
# on Ubuntu systems.
#
# Sets the following variables:
#  - CERNLIB_INCLUDE_DIRS: Include directories to be used to build
#    code against the CERNLIB libraries.
#  - CERNLIB_LIBRARIES: Libraries to link to use the CERNLIB code.
#

# Find the cernlib helper script:
find_program( CERNLIB_EXECUTABLE cernlib
  HINTS /cern/pro/bin )

# Find the include directories to use:
find_path( CERNLIB_INCLUDE_DIR NAMES cfortran/cfortran.h hbook.h
  HINTS /cern/pro/include )
set( CERNLIB_INCLUDE_DIRS ${CERNLIB_INCLUDE_DIR} )
mark_as_advanced( CERNLIB_INCLUDE_DIR )

# Get the list of libraries to link against:
execute_process( COMMAND ${CERNLIB_EXECUTABLE} ${CERNLIB_FIND_COMPONENTS}
  OUTPUT_VARIABLE CERNLIB_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE )
mark_as_advanced( CERNLIB_LIBRARIES )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CERNLIB DEFAULT_MSG
  CERNLIB_EXECUTABLE CERNLIB_INCLUDE_DIR CERNLIB_LIBRARIES )
mark_as_advanced( CERNLIB_FOUND )
