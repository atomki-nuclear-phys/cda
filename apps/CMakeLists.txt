#
# Configuration for building all the applications for CDA.
#

# Macro for building a console application
macro( cda_add_console_executable name )

  # Tell the user what's happening:
  message( STATUS "Configuring the build of console application ${name}" )

  # Find Qt:
  find_package( Qt456 COMPONENTS Core Xml REQUIRED )

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

  # Build the executable:
  add_executable( ${name} ${headers} ${sources} ${resources} )
  target_link_libraries( ${name} ${Qt456_LIBRARIES} cdacore cdadaq ${plugins}
    cdaapps )
  unset( plugins )
  unset( resources )
  set_property( TARGET ${name} PROPERTY FOLDER "apps/${name}" )

  # Group its source files:
  source_group( "" FILES ${headers} ${sources} )

  # Install the executable:
  install( TARGETS ${name} DESTINATION bin )

endmacro( cda_add_console_executable )

# Macro for building a GUI application
macro( cda_add_gui_executable name )

  # Parse optional arguments:
  cmake_parse_arguments( ARG "" "ICON" "" ${ARGN} )

  # Tell the user what's happening:
  message( STATUS "Configuring the build of GUI application ${name}" )

  # Find Qt:
  find_package( Qt456 COMPONENTS Core Gui Widgets Xml REQUIRED )

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
  target_link_libraries( ${name} ${Qt456_LIBRARIES} cdacore cdadaq cdagui
    ${plugins} cdaapps )
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

endmacro( cda_add_gui_executable )

# Macro for building a terminal/curses application
macro( cda_add_terminal_executable name )

   # Check if the cdacurses library is available:
   if( NOT TARGET cdacurses )
      message( STATUS "Not building ${name}" )
      return()
   endif()

   # Tell the user what's happening:
   message( STATUS "Configuring the build of terminal application ${name}" )

   # Find Qt:
   find_package( Qt456 COMPONENTS Core REQUIRED )

   # External(s) needed:
   find_package( Curses REQUIRED )

   # Collect the source files of the executable:
   file( GLOB headers *.h )
   file( GLOB sources *.cxx )

   # Build the executable:
   add_executable( ${name} ${headers} ${sources} )
   target_link_libraries( ${name} ${CURSES_LIBRARIES} ${Qt456_LIBRARIES}
      cdacore cdadaq cdacurses cdaapps )
   set_property( TARGET ${name} PROPERTY FOLDER "apps/${name}" )

   # Group its source files:
   source_group( "" FILES ${headers} ${sources} )

   # Install the executable:
   install( TARGETS ${name} DESTINATION bin )

endmacro( cda_add_terminal_executable )

# Build the common static library for the applications.
add_subdirectory( common )

# Build the console application(s):
add_subdirectory( cda-caen-reader )
add_subdirectory( cda-caen-vme-configurator )
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

# Build the terminal/curses application(s):
add_subdirectory( cda-caen-vme-terminal )
