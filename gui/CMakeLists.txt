# $Id$
#
# Configuration for building the CDA gui library.
#

# Greet the user:
message( STATUS "Configuring the build of cdagui" )

# Find Qt:
find_package( Qt5 QUIET COMPONENTS Core Network Gui Widgets )
if( Qt5_FOUND AND NOT CDA_USE_QT4 )
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
file( GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   msg/*.h device/*.h simple_daq/*.h stat/*.h common/*.h )
file( GLOB sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   msg/*.cxx device/*.cxx simple_daq/*.cxx stat/*.cxx
   common/*.cxx )
file( GLOB resources ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc )

# Remember the resource file name(s):
set_property( GLOBAL APPEND PROPERTY CDA_RESOURCES ${resources} )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )
# Generate RCC files automatically:
set( CMAKE_AUTORCC TRUE )

# Build the cdacore library:
add_library( cdagui STATIC ${headers} ${sources} ${resources} )
target_include_directories( cdagui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( cdagui PUBLIC ${qt_libraries} cdacore cdadaq )
set_target_properties( cdagui PROPERTIES
  VERSION 2.0.0 SOVERSION 2 )
target_compile_definitions( cdagui PRIVATE CDAGUI_LIBRARY )
set_property( TARGET cdagui PROPERTY FOLDER gui )

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
