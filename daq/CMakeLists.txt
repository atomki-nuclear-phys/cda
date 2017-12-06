#
# Configuration for building the CDA daq library.
#

# Greet the user:
message( STATUS "Configuring the build of cdadaq" )

# Find Qt:
find_package( Qt45 COMPONENTS Core Network Gui Widgets Xml REQUIRED )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdadaq library:
cda_add_library( cdadaq
   HEADERS common/*.h stat/*.h config/*.h moni/*.h
   SOURCES common/*.cxx stat/*.cxx config/*.cxx moni/*.cxx
   LINK_LIBRARIES ${Qt45_LIBRARIES} cdacore )
set_target_properties( cdadaq PROPERTIES
   VERSION 2.0.0
   SOVERSION 2 )
