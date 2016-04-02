# $Id$
#
# Configuration for building all the device plugins for CDA.
#

# Use the CMake argument parser code:
include( CMakeParseArguments )

# Macro for building a CDA device plugin
macro( add_cda_plugin name )

  # Parse optional arguments:
  cmake_parse_arguments( ARG "NO_AUTOMOC;NO_AUTORCC" "" "" ${ARGN} )

  # Greet the user:
  message( STATUS "Configuring the build of the ${name} plugin" )

  # Find Qt:
  find_package( Qt5 QUIET COMPONENTS Core Network Gui Widgets Xml )
  if( Qt5_FOUND )
    set( qt_libraries Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets
      Qt5::Xml )
  else()
    find_package( Qt4 QUIET COMPONENTS QtCore QtNetwork QtGui QtXml )
    if( NOT QT_FOUND )
      message( SEND_ERROR "No working version of Qt was found" )
      return()
    endif()
    set( qt_libraries Qt4::QtCore Qt4::QtNetwork Qt4::QtGui
      Qt4::QtXml )
  endif()

  # Generate MOC files automatically:
  if( NOT ARG_NO_AUTOMOC )
     set( CMAKE_AUTOMOC TRUE )
  endif()
  # Generate resource files automatically:
  if( NOT ARG_NO_AUTORCC )
     set( CMAKE_AUTORCC TRUE )
  endif()

  # Collect the source files of the plugin:
  file( GLOB headers *.h )
  file( GLOB sources *.cxx )
  file( GLOB resources ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc )

  # Build the plugin as a static library:
  add_library( ${name} STATIC ${headers} ${sources} ${resources} )
  target_include_directories( ${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )
  target_link_libraries( ${name} PRIVATE ${qt_libraries} cdacore cdadaq )
  target_compile_definitions( ${name} PRIVATE QT_STATICPLUGIN )

  # Remember the name and properties of the plugin:
  set_property( GLOBAL APPEND PROPERTY CDA_PLUGINS ${name} )
  set_property( GLOBAL APPEND PROPERTY CDA_RESOURCES ${resources} )

endmacro( add_cda_plugin )

# Build all the CAMAC plugins:
add_subdirectory( ad1000 )
add_subdirectory( ad2249a )
add_subdirectory( ad413a )
add_subdirectory( t2228a )
add_subdirectory( t4300b )

# Build all the CAEN digitiser plugins:
add_subdirectory( dt5740 )

# Build all the CAEN VME plugins:
add_subdirectory( v862 )
