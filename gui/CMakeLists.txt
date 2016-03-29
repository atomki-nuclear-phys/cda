# $Id$
#
# Configuration for building the CDA gui library.
#

# Greet the user:
message( STATUS "Configuring the build of cdagui" )

# Find Qt:
find_package( Qt5 QUIET COMPONENTS Core Network Gui Widgets )
if( Qt5_FOUND )
  message( STATUS "Using Qt5 for the build" )
  set( qt_libraries Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets )
else()
  find_package( Qt4 QUIET COMPONENTS QtCore QtNetwork QtGui )
  if( NOT QT_FOUND )
    message( SEND_ERROR "No working version of Qt was found" )
    return()
  endif()
  message( STATUS "Using Qt4 for the build" )
  set( qt_libraries Qt4::QtCore Qt4::QtNetwork Qt4::QtGui )
endif()

# Collect the source files of the library:
file( GLOB headers msg/*.h device/*.h simple_daq/*.h stat/*.h common/*.h )
file( GLOB sources msg/*.cxx device/*.cxx simple_daq/*.cxx stat/*.cxx
  common/*.cxx )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdacore library:
add_library( cdagui STATIC ${headers} ${sources} )
target_include_directories( cdagui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( cdagui PUBLIC ${qt_libraries} cdacore cdadaq )
set_target_properties( cdagui PROPERTIES
  VERSION 2.0.0 SOVERSION 2 )
target_compile_definitions( cdagui PRIVATE CDAGUI_LIBRARY )
