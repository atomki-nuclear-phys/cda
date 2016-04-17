# $Id$
#
# Configuration for building all the applications for CDA.
#

# Macro for building a console application
macro( add_cda_console_app name )

  # Tell the user what's happening:
  message( STATUS "Configuring the build of console application ${name}" )

  # Find Qt:
  find_package( Qt5 QUIET COMPONENTS Core Xml )
  if( Qt5_FOUND AND NOT CDA_USE_QT4 )
    set( qt_libraries Qt5::Core Qt5::Xml )
  else()
    find_package( Qt4 QUIET COMPONENTS QtCore QtXml )
    if( NOT QT_FOUND )
      message( SEND_ERROR "No working version of Qt was found" )
      return()
    endif()
    set( qt_libraries Qt4::QtCore Qt4::QtXml )
  endif()

  # Collect the source files of the executable:
  file( GLOB headers *.h ${CMAKE_SOURCE_DIR}/apps/win32/*.h )
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
  set_property( TARGET ${name} PROPERTY FOLDER "apps/${name}" )

  # Group its source files:
  source_group( "" FILES ${headers} ${sources} )

  # Install the executable:
  install( TARGETS ${name} DESTINATION bin )

endmacro( add_cda_console_app )

# Macro for building a GUI application
macro( add_cda_gui_app name )

  # Parse optional arguments:
  cmake_parse_arguments( ARG "" "ICON" "" ${ARGN} )

  # Tell the user what's happening:
  message( STATUS "Configuring the build of GUI application ${name}" )

  # Build an application bundle for MacOS X:
  include( BundleUtilities )

  # Find Qt:
  find_package( Qt5 QUIET COMPONENTS Core Gui Widgets Xml )
  if( Qt5_FOUND AND NOT CDA_USE_QT4 )
    set( qt_libraries Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Xml )
  else()
    find_package( Qt4 QUIET COMPONENTS QtCore QtGui QtXml )
    if( NOT QT_FOUND )
      message( SEND_ERROR "No working version of Qt was found" )
      return()
    endif()
    set( qt_libraries Qt4::QtCore Qt4::QtGui Qt4::QtXml )
  endif()

  # Collect the source files of the executable:
  file( GLOB headers *.h ${CMAKE_SOURCE_DIR}/apps/win32/*.h )
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

  # Decide what kind of an application to build:
  set( OS_BUNDLE )
  if( APPLE )
     set( OS_BUNDLE MACOSX_BUNDLE )
  elseif( WIN32 )
     set( OS_BUNDLE WIN32 )
  endif()
  
  # Build the executable:
  add_executable( ${name} ${OS_BUNDLE} ${headers} ${sources}
    ${resources} ${ARG_ICON} )
  target_link_libraries( ${name} ${qt_libraries} cdacore cdadaq cdagui
    ${plugins} )
  unset( plugins )
  unset( resources )
  unset( OS_BUNDLE )
  set_property( TARGET ${name} PROPERTY FOLDER "apps/${name}" )

  # Group its source files:
  source_group( "" FILES ${headers} ${sources} )

  # Set up the icon for the application, if one was specified:
  if( ARG_ICON AND APPLE )
     set_source_files_properties( ${ARG_ICON} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources" )
     get_filename_component( fname ${ARG_ICON} NAME )
     set_target_properties( ${name} PROPERTIES
        MACOSX_BUNDLE_ICON_FILE ${fname} )
     unset( fname )
  endif()

  # Install the executable:
  install( TARGETS ${name} DESTINATION bin )

endmacro( add_cda_gui_app )

# Build the console application(s):
add_subdirectory( cda-caen-reader )
add_subdirectory( cda-caen-vme-reader )
add_subdirectory( cda-camac-reader )
add_subdirectory( cda-config-server )
add_subdirectory( cda-glomem-writer )
add_subdirectory( cda-hbook-writer )
add_subdirectory( cda-raw-writer )
add_subdirectory( cda-root-writer )

# Build the GUI application(s):
add_subdirectory( cda-caen-daq )
add_subdirectory( cda-caen-vme-daq )
add_subdirectory( cda-camac-daq )
add_subdirectory( cda-config-editor )
add_subdirectory( cda-msgserver )
add_subdirectory( cda-qt-monitoring )
add_subdirectory( cda-stat-server )
