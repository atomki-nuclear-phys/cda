# $Id$
#
# Module finding the cc32 CAMAC interface library for the build.
#

# Find the cc32 header:
find_path( CC32_INCLUDE_DIR
  NAMES libcc32.h )
set( CC32_INCLUDE_DIRS ${CC32_INCLUDE_DIR} )
mark_as_advanced( CC32_INCLUDE_DIR )

# Find the cc32 library:
find_library( CC32_LIBRARY NAMES cc32 )
set( CC32_LIBRARIES ${CC32_LIBRARY} )
mark_as_advanced( CC32_LIBRARY )

# Print the usual find_package messages:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( cc32 DEFAULT_MSG
  CC32_LIBRARY CC32_INCLUDE_DIR )
mark_as_advanced( CC32_FOUND )
