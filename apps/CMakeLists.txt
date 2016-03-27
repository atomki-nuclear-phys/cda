# $Id$
#
# Configuration for building all the applications for CDA.
#

# Macro for building a console application
macro( add_cda_console_app name )

  # Tell the user what's happening:
  message( STATUS "Configuring the build of console application ${name}" )

  # Find Qt:
  find_package( Qt5 COMPONENTS Core Xml )
  if( Qt5_FOUND )
    set( qt_libraries Qt5::Core Qt5::Xml )
  else()
    find_package( Qt4 COMPONENTS QtCore QtXml )
    if( NOT Qt4_FOUND )
      message( ERROR "No working version of Qt was found" )
      return()
    endif()
    set( qt_libraries Qt4::QtCore Qt4::QtXml )
  endif()

  # Collect the source files of the executable:
  file( GLOB headers *.h )
  file( GLOB sources *.cxx )

  # Get the names and properties of the available plugins:
  get_property( pluginsAvailable GLOBAL PROPERTY CDA_PLUGINS SET )
  if( ${pluginsAvailable} )
    get_property( plugins GLOBAL PROPERTY CDA_PLUGINS )
  else()
    set( plugins )
  endif()
  unset( pluginsAvailable )
  get_property( resourcesAvailable GLOBAL PROPERTY CDA_RESOURCES SET )
  if( ${resourcesAvailable} )
    get_property( resources GLOBAL PROPERTY CDA_RESOURCES )
  else()
    set( resources )
  endif()
  unset( resourcesAvailable )

  # Generate MOC files automatically:
  set( CMAKE_AUTOMOC TRUE )
  # Generate resource files automatically:
  set( CMAKE_AUTORCC TRUE )

  # Build the executable:
  add_executable( ${name} ${headers} ${sources} ${resources} )
  target_link_libraries( ${name} ${qt_libraries} cdacore cdadaq ${plugins} )
  unset( plugins )
  unset( resources )

endmacro( add_cda_console_app )

# Macro for building a GUI application
macro( add_cda_gui_app name )

  # Tell the user what's happening:
  message( STATUS "Configuring the build of GUI application ${name}" )

  # Find Qt:
  find_package( Qt5 COMPONENTS Core Gui Widgets Xml )
  if( Qt5_FOUND )
    set( qt_libraries Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml )
  else()
    find_package( Qt4 COMPONENTS QtCore QtGui QtXml )
    if( NOT Qt4_FOUND )
      message( ERROR "No working version of Qt was found" )
      return()
    endif()
    set( qt_libraries Qt4::QtCore Qt4::QtGui Qt4::QtXml )
  endif()

  # Collect the source files of the executable:
  file( GLOB headers *.h )
  file( GLOB sources *.cxx *.qrc )

  # Get the names of the available plugins:
  get_property( pluginsAvailable GLOBAL PROPERTY CDA_PLUGINS SET )
  if( ${pluginsAvailable} )
    get_property( plugins GLOBAL PROPERTY CDA_PLUGINS )
  else()
    set( plugins )
  endif()
  unset( pluginsAvailable )
  get_property( resourcesAvailable GLOBAL PROPERTY CDA_RESOURCES SET )
  if( ${resourcesAvailable} )
    get_property( resources GLOBAL PROPERTY CDA_RESOURCES )
  else()
    set( resources )
  endif()
  unset( resourcesAvailable )

  # Generate MOC files automatically:
  set( CMAKE_AUTOMOC TRUE )
  # Generate resource files automatically:
  set( CMAKE_AUTORCC TRUE )

  # Build the executable:
  add_executable( ${name} ${headers} ${sources} ${resources} )
  target_link_libraries( ${name} ${qt_libraries} cdacore cdadaq cdagui
    ${plugins} )
  unset( plugins )
  unset( resources )

endmacro( add_cda_gui_app )

# Build the console application(s):
add_subdirectory( cda-caen-reader )
add_subdirectory( cda-camac-reader )
add_subdirectory( cda-config-server )
add_subdirectory( cda-glomem-writer )
add_subdirectory( cda-hbook-writer )
add_subdirectory( cda-raw-writer )
add_subdirectory( cda-root-writer )

# Build the GUI application(s):
add_subdirectory( cda-caen-daq )
add_subdirectory( cda-camac-daq )
add_subdirectory( cda-config-editor )
add_subdirectory( cda-msgserver )
add_subdirectory( cda-qt-monitoring )
add_subdirectory( cda-stat-server )
