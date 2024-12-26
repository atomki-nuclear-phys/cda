#
# Main CMake configuration for the CDA project. Used to set up the
# build of the entire project.
#

# Set up the project:
cmake_minimum_required( VERSION 3.12...3.31 )
project( CDA VERSION 1.3.0 LANGUAGES CXX )

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
option( CDA_USE_QT4
  "Force the usage of Qt4, even when Qt5/6 is available" FALSE )
option( CDA_USE_QT5
  "Force the usage of Qt5, even when Qt6 is available" FALSE )
option( CDA_FAIL_ON_WARNINGS
  "Fail the build if any warnings are generated" FALSE )

# Set up the output directories for the build results:
include( GNUInstallDirs )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}" CACHE PATH
  "Directory for the built binaries" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH
  "Directory for the built libraries" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}" CACHE PATH
  "Directory for the built static libraries" )

# Find the CDA CMake helper code:
list( APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake )
include( CDAFunctions )

# Set the compile flags.
set( CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to use for the build" )
set( CMAKE_CXX_EXTENSIONS FALSE CACHE BOOL "(Dis)allow compiler extensions" )

# Turn on a number of warnings for the "known compilers".
if( ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU" ) OR
    ( "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" ) )

  # Turn on a schizophrenic amount of warnings.
  cda_add_flag( CMAKE_CXX_FLAGS "-Wall" )
  cda_add_flag( CMAKE_CXX_FLAGS "-Wextra" )

  # Fail on warnings, if asked for that behaviour.
  if( CDA_FAIL_ON_WARNINGS )
    cda_add_flag( CMAKE_CXX_FLAGS "-Werror" )
  endif()
endif()

# Turn on using folders in certain IDEs (Xcode,VS):
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

# Build the main libraries of the project:
add_subdirectory( core )
add_subdirectory( daq )
add_subdirectory( gui )
add_subdirectory( curses )

# Build the device plugins:
add_subdirectory( devices )

# Build the applications:
add_subdirectory( apps )

# Install the setup files:
install( FILES setup.sh setup.csh DESTINATION . )
