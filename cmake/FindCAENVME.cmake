# $Id$
#
# Module finding the CAEN VME library for the build.
#

# Find the CAEN VME headers:
find_path( CAENVME_INCLUDE_DIR
  NAMES CAENVMElib.h CAENVMEoslib.h CAENVMEtypes.h
  PATHS "/Program Files/CAEN/VME/include"
        "/Program Files (x86)/CAEN/VME/include" )
set( CAENVME_INCLUDE_DIRS ${CAENVME_INCLUDE_DIR} )
mark_as_advanced( CAENVME_INCLUDE_DIR )

# Find the CAEN VME library:
find_library( CAENVME_LIBRARY NAMES CAENVME CAENVMElib
  PATHS "/Program Files/CAEN/VME/lib/x86_64"
        "/Program Files (x86)/CAEN/VME/lib/x86_64" )
set( CAENVME_LIBRARIES ${CAENVME_LIBRARY} )
mark_as_advanced( CAENVME_LIBRARY )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CAENVME DEFAULT_MSG
  CAENVME_LIBRARY CAENVME_INCLUDE_DIR )
mark_as_advanced( CAENVME_FOUND )

# On Windows use the WIN32 compiler definition:
if( WIN32 )
   add_definitions( -DWIN32 )
endif()

# On UNIX platforms use LINUX:
if( UNIX )
   add_definitions( -DLINUX )
endif()
