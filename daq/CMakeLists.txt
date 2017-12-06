#
# Configuration for building the CDA daq library.
#

# Greet the user:
message( STATUS "Configuring the build of cdadaq" )

# Find Qt:
find_package( Qt45 COMPONENTS Core Network Gui Widgets Xml REQUIRED )

# Collect the source files of the library:
file( GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   common/*.h stat/*.h config/*.h moni/*.h )
file( GLOB sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   common/*.cxx stat/*.cxx config/*.cxx moni/*.cxx )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdacore library:
add_library( cdadaq STATIC ${headers} ${sources} )
target_include_directories( cdadaq PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( cdadaq ${Qt45_LIBRARIES} cdacore )
set_target_properties( cdadaq PROPERTIES
  VERSION 2.0.0 SOVERSION 2 )
target_compile_definitions( cdadaq PRIVATE CDADAQ_LIBRARY )
set_property( TARGET cdadaq PROPERTY FOLDER daq )

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
