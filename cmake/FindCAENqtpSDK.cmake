# $Id$
#
# Module finding the CAEN QTP libraries for the build.
#

# Find the CAEN QTP headers:
find_path( CAENQTPSDK_INCLUDE_DIR
  NAMES cvt_V792.h cvt_board_commons.h cvt_common_defs.h )
set( CAENQTPSDK_INCLUDE_DIRS ${CAENQTPSDK_INCLUDE_DIR} )
mark_as_advanced( CAENQTPSDK_INCLUDE_DIR )

# Find the CAEN QTP library:
find_library( CAENQTPSDK_LIBRARY NAMES CAENqtpSDK )
set( CAENQTPSDK_LIBRARIES ${CAENQTPSDK_LIBRARY} )
mark_as_advanced( CAENQTPSDK_LIBRARY )

# The CAEN QTP library also needs the CAEN VME library:
find_package( CAENVME )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CAENqtpSDK DEFAULT_MSG
  CAENQTPSDK_LIBRARY CAENQTPSDK_INCLUDE_DIR
  CAENVME_FOUND )
mark_as_advanced( CAENQTPSDK_FOUND )

# If everything was found, extend the include and library variables
# a bit:
if( CAENQTPSDK_FOUND )
  list( APPEND CAENQTPSDK_INCLUDE_DIRS
    ${CAENVME_INCLUDE_DIRS} )
  list( APPEND CAENQTPSDK_LIBRARIES
    ${CAENVME_LIBRARIES} )
endif()
