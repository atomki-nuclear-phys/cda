#
# Configuration for building the CDA tui library.
#

# Greet the user:
message(STATUS "Configuring the build of cdatui")

# Silence warnings about CMP0135.
if(POLICY CMP0135)
   cmake_policy(SET CMP0135 NEW)
endif()

# Download and build FTXUI as part of this project.
include(FetchContent)
set(CDA_FTXUI_SOURCE
   "URL;https://github.com/ArthurSonzogni/FTXUI/archive/refs/tags/v5.0.0.tar.gz;URL_MD5;1f7403ed40d5387a554ab79c71f15086"
   CACHE STRING "Source for FTXUI")
FetchContent_Declare(FTXUI ${CDA_FTXUI_SOURCE})
FetchContent_Populate(FTXUI)
add_subdirectory("${ftxui_SOURCE_DIR}" "${ftxui_BINARY_DIR}" EXCLUDE_FROM_ALL)

# Build the cdatui library:
cda_add_library(cdatui
   HEADERS msg/*.h
   SOURCES msg/*.cxx
   LINK_LIBRARIES cdacore cdadaq component)
set_target_properties(cdatui PROPERTIES
   VERSION 2.0.0
   SOVERSION 2)
