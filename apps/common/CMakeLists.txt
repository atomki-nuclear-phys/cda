#
# Configuration for building the cda-apps-common static library.
#

# Find Qt.
find_package(Qt456 COMPONENTS Core Xml REQUIRED)

# Build a common static library for the applications.
add_library(cdaapps STATIC
   "plugins.h"
   "readConfig.h"
   "readConfig.cxx")
target_link_libraries(cdaapps ${Qt456_LIBRARIES} cdacore cdadaq)
