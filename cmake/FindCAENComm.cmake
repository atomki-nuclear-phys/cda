# $Id$
#
# Module finding the CAEN comm libraries for the build.
#

# Find the CAEN comm header:
find_path( CAENCOMM_INCLUDE_DIR
  NAMES CAENComm.h
  PATHS "/Program Files/CAEN/Comm/include"
        "/Program Files (x86)/CAEN/Comm/include" )
set( CAENCOMM_INCLUDE_DIRS ${CAENCOMM_INCLUDE_DIR} )
mark_as_advanced( CAENCOMM_INCLUDE_DIR )

# Find the CAEN comm library:
find_library( CAENCOMM_LIBRARY NAMES CAENComm
  PATHS "/Program Files/CAEN/Comm/lib/x86_64"
        "/Program Files (x86)/CAEN/Comm/lib/x86_64" )
set( CAENCOMM_LIBRARIES ${CAENCOMM_LIBRARY} )
mark_as_advanced( CAENCOMM_LIBRARY )

# The CAEN comm library also needs the CAEN VME library:
find_package( CAENVME )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CAENComm DEFAULT_MSG
  CAENCOMM_LIBRARY CAENCOMM_INCLUDE_DIR
  CAENVME_FOUND )
mark_as_advanced( CAENCOMM_FOUND )

# If everything was found, extend the include and library variables
# a bit:
if( CAENCOMM_FOUND )
  list( APPEND CAENCOMM_INCLUDE_DIRS
    ${CAENVME_INCLUDE_DIRS} )
  list( APPEND CAENCOMM_LIBRARIES
    ${CAENVME_LIBRARIES} )
endif()

# On Windows use the WIN32 compiler definition:
if( WIN32 )
   add_definitions( -DWIN32 )
endif()
