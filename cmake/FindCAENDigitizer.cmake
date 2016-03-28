# $Id$
#
# Module finding the CAEN digitizer libraries for the build.
#

# Find the CAEN digitizer headers:
find_path( CAENDIGITIZER_INCLUDE_DIR
  NAMES CAENDigitizer.h CAENDigitizerType.h
  HINTS "/Program Files/CAEN/Digitizers/Library/include" )
set( CAENDIGITIZER_INCLUDE_DIRS ${CAENDIGITIZER_INCLUDE_DIR} )
mark_as_advanced( CAENDIGITIZER_INCLUDE_DIR )

# Find the CAEN digitizer library:
find_library( CAENDIGITIZER_LIBRARY NAMES CAENDigitizer
  HINTS "/Program Files/CAEN/Digitizers/Library/lib/x86_64" )
set( CAENDIGITIZER_LIBRARIES ${CAENDIGITIZER_LIBRARY} )
mark_as_advanced( CAENDIGITIZER_LIBRARY )

# The CAEN digitizer library also needs the CAEN VME and
# Comm libraries:
find_package( CAENVME )
find_package( CAENComm )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CAENDigitizer DEFAULT_MSG
  CAENDIGITIZER_LIBRARY CAENDIGITIZER_INCLUDE_DIR
  CAENVME_FOUND CAENCOMM_FOUND )
mark_as_advanced( CAENDIGITIZER_FOUND )

# If everything was found, extend the include and library variables
# a bit:
if( CAENDIGITIZER_FOUND )
  list( APPEND CAENDIGITIZER_INCLUDE_DIRS
    ${CAENVME_INCLUDE_DIRS} ${CAENCOMM_INCLUDE_DIRS} )
  list( APPEND CAENDIGITIZER_LIBRARIES
    ${CAENVME_LIBRARIES} ${CAENCOMM_LIBRARIES} )
endif()

# On Windows use the WIN32 compiler definition:
if( WIN32 )
   add_definitions( -DWIN32 )
endif()
