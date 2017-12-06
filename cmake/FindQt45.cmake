#
# Module finding either Qt4 or Qt5 for the build of CDA.
#

# Look for Qt5 first:
find_package( Qt5 QUIET COMPONENTS ${Qt45_FIND_COMPONENTS} )

# If Qt5 was found, set up the variables we need, using it:
if( Qt5_FOUND AND NOT CDA_USE_QT4 )
   get_target_property( _qmakePath Qt5::qmake IMPORTED_LOCATION )
   set( Qt45_LIBRARIES )
   foreach( lib ${Qt45_FIND_COMPONENTS} )
      list( APPEND Qt45_LIBRARIES "Qt5::${lib}" )
   endforeach()
   set( _version "${Qt5_VERSION}" )
else()
   # Compose the component names, from the "Qt5 style" names given by the user:
   set( _components )
   foreach( comp ${Qt45_FIND_COMPONENTS} )
      # Ignore the "Widgets" library, as it was only introduced in Qt5.
      if( NOT "${comp}" STREQUAL "Widgets" )
         list( APPEND _components "Qt${comp}" )
      endif()
   endforeach()

   # Look for Qt4:
   find_package( Qt4 QUIET COMPONENTS ${_components} )

   # If it was found, set up the variables we need:
   if( Qt4_FOUND )
      set( _qmakePath ${QT_QMAKE_EXECUTABLE} )
      set( Qt45_LIBRARIES )
      foreach( lib ${_components} )
         list( APPEND Qt45_LIBRARIES "Qt4::${lib}" )
      endforeach()
      set( _version "${Qt4_VERSION}" )
   endif()

   # Clean up:
   unset( _components )
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qt45
   FOUND_VAR Qt45_FOUND
   REQUIRED_VARS _qmakePath Qt45_LIBRARIES
   VERSION_VAR _version )

# Clean up:
unset( _extraArgs )
unset( _qmakePath )
unset( _version )
