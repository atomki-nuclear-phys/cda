# $Id$
#
# Main CMake configuration for the CDA project. Used to set up the
# build of the entire project.
#

# Set up the project:
cmake_minimum_required( VERSION 2.8 )
project( CDA )

# Declare the options for controlling what externals CDA should be
# built against. If a given external is not found, that's not a
# problem. So by default everything is turned on.
option( CDA_USE_ROOT "Build CDA against ROOT if it's available" TRUE )
option( CDA_USE_CERNLIB "Build CDA against CERNLIB if it's available"
  TRUE )
option( CDA_USE_CC32
  "Build CDA against the CC32 CAMAC library if it's available" TRUE )
option( CDA_USE_CAENDIGITIZER
  "Build CDA against the CAEN digitizer libraries if they are available"
  TRUE )
option( CDA_USE_CAENQTPSDK
  "Build CDA against the CAEN QTP libraries if they are available" TRUE )

# Set up the output directories for the build results:
if( UNIX )
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    CACHE PATH "Directory to store shared libraries" )
else()
  set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    CACHE PATH "Directory to store shared libraries" )
endif()
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  CACHE PATH "Directory to store static libraries" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  CACHE PATH "Directory to store executables" )
foreach( config DEBUG RELEASE MINSIZEREL RELWITHDEBINFO )
  if( UNIX )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${CMAKE_BINARY_DIR}/lib"
      CACHE PATH "Directory to store shared libraries" )
  else()
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} "${CMAKE_BINARY_DIR}/bin"
      CACHE PATH "Directory to store shared libraries" )
  endif()
  set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${config} "${CMAKE_BINARY_DIR}/lib"
    CACHE PATH "Directory to store static libraries" )
  set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} "${CMAKE_BINARY_DIR}/lib"
    CACHE PATH "Directory to store executables" )
endforeach()
if( UNIX )
  set( CMAKE_DEVICE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/dev"
    CACHE PATH "Directory to store device plugins" )
else()
  set( CMAKE_DEVICE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    CACHE PATH "Directory to store device plugins" )
endif()

# Find the CDA CMake helper code:
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )

# The build needs c++11 mode:
set( CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to use for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow compiler extensions" )
if( ${CMAKE_VERSION} VERSION_LESS 3.1 )
  set( CMAKE_CXX_FLAGS "-std=c++0x" )
  foreach( config DEBUG RELEASE MINSIZEREL RELWITHDEBINFO )
    set( CMAKE_CXX_FLAGS_${config} "-std=c++0x" )
  endforeach()
endif()

# Build the main libraries of the project:
add_subdirectory( core )
