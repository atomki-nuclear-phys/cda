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

# Find Qt:
find_package( Qt456 COMPONENTS Core REQUIRED )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdacurses library:
cda_add_library( cdacurses
   HEADERS msg/*.h
   SOURCES msg/*.cxx
   INCLUDE_DIRS ${CURSES_INCLUDE_DIRS}
   LINK_LIBRARIES ${CURSES_LIBRARIES} ${Qt456_LIBRARIES} cdacore )
set_target_properties( cdacurses PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
