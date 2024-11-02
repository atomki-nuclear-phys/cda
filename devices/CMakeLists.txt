#
# Configuration for building all the device plugins for CDA.
#

# Use the CMake argument parser code:
include( CMakeParseArguments )

# Macro for building a CDA device plugin
macro( cda_add_plugin name )

  # Parse optional arguments:
  cmake_parse_arguments( ARG "NO_AUTOMOC;NO_AUTORCC" "" "" ${ARGN} )

  # Greet the user:
  message( STATUS "Configuring the build of the ${name} plugin" )

  # Find Qt:
  find_package( Qt456 COMPONENTS Core Network Gui Widgets Xml REQUIRED )

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
  target_link_libraries( ${name} ${Qt45_LIBRARIES} cdacore cdadaq )
  target_compile_definitions( ${name} PRIVATE QT_STATICPLUGIN )
  set_property( TARGET ${name} PROPERTY FOLDER "devices/${name}" )

  # Group its source files:
  source_group( "" FILES ${headers} ${sources} )

  # Remember the name and properties of the plugin:
  set_property( GLOBAL APPEND PROPERTY CDA_PLUGINS ${name} )
  set_property( GLOBAL APPEND PROPERTY CDA_RESOURCES ${resources} )

endmacro( cda_add_plugin )

# Build all the CAMAC plugins:
add_subdirectory( ad1000 )
add_subdirectory( ad2249a )
add_subdirectory( ad413a )
add_subdirectory( t2228a )
add_subdirectory( t4300b )

# Build all the CAEN digitiser plugins:
add_subdirectory( dt5740 )

# Build all the CAEN VME plugins:
add_subdirectory( v775 )
add_subdirectory( v785 )
add_subdirectory( v792 )
add_subdirectory( v862 )
add_subdirectory( v812 )
