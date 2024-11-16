#
# Module finding either Qt4 or Qt5 for the build of CDA.
#

# Look for Qt 6 first.
find_package( Qt6 QUIET COMPONENTS ${Qt456_FIND_COMPONENTS} )

# If Qt6 was found, set up the variables we need, using it.
if( Qt6_FOUND AND NOT ( CDA_USE_QT5 OR CDA_USE_QT4 ) )
   get_target_property( _qmakePath Qt6::qmake IMPORTED_LOCATION )
   set( Qt456_LIBRARIES )
   foreach( lib ${Qt456_FIND_COMPONENTS} )
      list( APPEND Qt456_LIBRARIES "Qt6::${lib}" )
   endforeach()
   set( Qt456_VERSION "6" )
else()
   # Look for Qt5.
   find_package( Qt5 QUIET COMPONENTS ${Qt456_FIND_COMPONENTS} )
   # If Qt5 was found, set up the variables we need, using it.
   if( Qt5_FOUND AND NOT CDA_USE_QT4 )
      get_target_property( _qmakePath Qt5::qmake IMPORTED_LOCATION )
      set( Qt456_LIBRARIES )
      foreach( lib ${Qt456_FIND_COMPONENTS} )
         list( APPEND Qt456_LIBRARIES "Qt5::${lib}" )
      endforeach()
      set( Qt456_VERSION "${Qt5_VERSION}" )
   else()
      # Compose the component names, from the "Qt5/6 style" names given by the
      # user.
      set( _components )
      foreach( comp ${Qt456_FIND_COMPONENTS} )
         # Ignore the "Widgets" library, as it was only introduced in Qt5.
         if( NOT "${comp}" STREQUAL "Widgets" )
            list( APPEND _components "Qt${comp}" )
         endif()
      endforeach()

      # Look for Qt4.
      find_package( Qt4 QUIET COMPONENTS ${_components} )

      # If it was found, set up the variables we need:
      if( Qt4_FOUND )
         set( _qmakePath ${QT_QMAKE_EXECUTABLE} )
         set( Qt456_LIBRARIES )
         foreach( lib ${_components} )
            list( APPEND Qt456_LIBRARIES "Qt4::${lib}" )
         endforeach()
         set( Qt456_VERSION "${QTVERSION}" )
      endif()

      # Clean up:
      unset( _components )
   endif()
endif()

# Handle the standard find_package arguments:
include( FindPackageHandleStandardArgs )
find_package_handle_standard_args( Qt456
   FOUND_VAR Qt456_FOUND
   REQUIRED_VARS _qmakePath Qt456_LIBRARIES
   VERSION_VAR Qt456_VERSION )

# Clean up:
unset( _extraArgs )
unset( _qmakePath )
