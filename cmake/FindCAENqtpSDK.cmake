# $Id$
#
# Module finding the CAEN QTP libraries for the build.
#

# Find the CAEN QTP headers:
find_path( _v792_dir
  NAMES cvt_V792.h 
  PATHS "/Program Files/CAEN/CAENVMEToolBox/CAENqtp/SDK/CAENqtpSDK"
        "/Program Files (x86)/CAEN/CAENVMEToolBox/CAENqtp/SDK/CAENqtpSDK" )
find_path( _common_dir
  NAMES cvt_board_commons.h cvt_common_defs.h
  PATHS "/Program Files/CAEN/CAENVMEToolBox/CAENqtp/SDK/CAENqtpSDK/common/src"
        "/Program Files (x86)/CAEN/CAENVMEToolBox/CAENqtp/SDK/CAENqtpSDK/common/src" )
set( CAENQTPSDK_INCLUDE_DIRS ${_v792_dir} ${_common_dir} )
list( REMOVE_DUPLICATES CAENQTPSDK_INCLUDE_DIRS )
unset( _v792_dir )
mark_as_advanced( _v792_dir )
unset( _common_dir )
mark_as_advanced( _common_dir )

# Find the CAEN QTP library:
find_library( CAENQTPSDK_LIBRARY NAMES CAENqtpSDK
  PATHS "/Program Files/CAEN/CAENVMEToolBox/CAENqtp/SDK/bin"
        "/Program Files (x86)/CAEN/CAENVMEToolBox/CAENqtp/SDK/bin" )
set( CAENQTPSDK_LIBRARIES ${CAENQTPSDK_LIBRARY} )
mark_as_advanced( CAENQTPSDK_LIBRARY )

# The CAEN QTP library also needs the CAEN VME library:
find_package( CAENVME )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( CAENqtpSDK DEFAULT_MSG
  CAENQTPSDK_LIBRARY CAENQTPSDK_INCLUDE_DIRS
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
