# $Id$
#
# Configuration for building all the device plugins for CDA.
#

# Macro for building a CDA device plugin
macro( add_cda_plugin name )

  # Greet the user:
  message( STATUS "Configuring the build of the ${name} plugin" )

  # Find Qt:
  find_package( Qt5 COMPONENTS Core Network Gui Widgets Xml )
  if( Qt5_FOUND )
    set( qt_libraries Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets
      Qt5::Xml )
  else()
    find_package( Qt4 COMPONENTS QtCore QtNetwork QtGui QtXml )
    if( NOT Qt4_FOUND )
      message( ERROR "No working version of Qt was found" )
      return()
    endif()
    set( qt_libraries Qt4::QtCore Qt4::QtNetwork Qt4::QtGui
      Qt4::QtXml )
  endif()

  # Set the output directory for the plugin library:
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_DEVICE_OUTPUT_DIRECTORY} )
  foreach( config DEBUG RELEASE MINSIZEREL RELWITHDEBINFO )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config}
      ${CMAKE_DEVICE_OUTPUT_DIRECTORY} )
  endforeach()

  # Generate MOC files automatically:
  set( CMAKE_AUTOMOC TRUE )
  # Generate resource files automatically:
  set( CMAKE_AUTORCC TRUE )

  # Collect the source files of the plugin:
  file( GLOB headers *.h )
  file( GLOB sources *.cxx *.qrc )

  # Build a module library out of the sources:
  add_library( ${name} MODULE ${headers} ${sources} )
  target_include_directories( ${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} )
  target_link_libraries( ${name} PRIVATE ${qt_libraries} cdacore cdadaq )

endmacro( add_cda_plugin )

# Build all the CAMAC plugins:
add_subdirectory( ad1000 )
add_subdirectory( ad2249a )
add_subdirectory( ad413a )
add_subdirectory( t2228a )
add_subdirectory( t4300b )

# Build all the CAEN digitiser plugins:
add_subdirectory( dt5740 )

# Build all the VME plugins:
add_subdirectory( s9418t )
