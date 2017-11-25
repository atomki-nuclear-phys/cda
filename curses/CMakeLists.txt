# $Id$
#
# Configuration for building the CDA curses library.
#

# Greet the user:
message( STATUS "Configuring the build of cdacurses" )

# Find the Curses libraries:
find_package( Curses )
if( NOT CURSES_FOUND )
   message( STATUS "Curses library not available, skipping cdacurses build" )
   return()
endif()

# Collect the source files of the library:
file( GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   msg/*.h )
file( GLOB sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   msg/*.cxx )

# Build the cdacurses library:
add_library( cdacurses STATIC ${headers} ${sources} )
target_include_directories( cdacurses PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
   ${CURSES_INCLUDE_DIRS} )
target_link_libraries( cdacurses PUBLIC ${CURSES_LIBRARIES} cdacore )
set_target_properties( cdacurses PROPERTIES
  VERSION 2.0.0 SOVERSION 2 )
target_compile_definitions( cdacurses PRIVATE CDACURSES_LIBRARY )
set_property( TARGET cdacurses PROPERTY FOLDER curses )

# Group its source files:
foreach( f ${headers} ${sources} )
   # Get the file's path:
   get_filename_component( _path ${f} PATH )
   # Replace the forward slashes with double backward slashes:
   string( REPLACE "/" "\\\\" _group "${_path}" )
   # Put the file into the right group:
   source_group( ${_group} FILES ${f} )
endforeach()
unset( _path )
unset( _group )
