#
# Configuration for building the CDA daq library.
#

# Greet the user:
message( STATUS "Configuring the build of cdadaq" )

# Find Qt:
find_package( Qt456 COMPONENTS Core Network Gui Widgets Xml REQUIRED )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdadaq library:
cda_add_library( cdadaq
   HEADERS common/*.h stat/*.h config/*.h moni/*.h
   SOURCES common/*.cxx stat/*.cxx config/*.cxx moni/*.cxx
   LINK_LIBRARIES ${Qt456_LIBRARIES} cdacore )
set_target_properties( cdadaq PROPERTIES
   VERSION ${PROJECT_VERSION}
   SOVERSION ${PROJECT_VERSION_MAJOR} )
