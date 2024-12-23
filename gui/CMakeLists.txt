#
# Configuration for building the CDA gui library.
#

# Greet the user:
message( STATUS "Configuring the build of cdagui" )

# Find Qt:
find_package( Qt456 COMPONENTS Core Network Gui Widgets REQUIRED )

# Collect the resource file(s):
file( GLOB resources ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc )

# Remember the resource file name(s):
set_property( GLOBAL APPEND PROPERTY CDA_RESOURCES ${resources} )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )
# Generate RCC files automatically:
set( CMAKE_AUTORCC TRUE )

# Build the cdagui library:
cda_add_library( cdagui
   HEADERS msg/*.h device/*.h simple_daq/*.h stat/*.h common/*.h
   SOURCES msg/*.cxx device/*.cxx simple_daq/*.cxx stat/*.cxx common/*.cxx
   ${resources}
   LINK_LIBRARIES ${Qt456_LIBRARIES} cdacore cdadaq )
set_target_properties( cdagui PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
