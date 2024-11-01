#
# Configuration for building the CDA core library.
#

# Greet the user:
message( STATUS "Configuring the build of cdacore" )

# Find Qt:
find_package( Qt45 COMPONENTS Core Network Gui Widgets REQUIRED )

# Optional I/O libraries to use for the build:
find_package( ROOT )
find_package( CERNLIB COMPONENTS packlib )

# Optional device libraries to use for the build:
find_package( cc32 )
find_package( CAENDigitizer )
find_package( CAENqtpSDK )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdacore library:
cda_add_library( cdacore
   HEADERS fifo/*.h msg/*.h event/*.h device/*.h device/*.icc camac/*.h caen/*.h
   cernlib/*.h root/*.h root/*.icc common/*.h i18n/*.h tclap/*.h
   SOURCES fifo/*.cxx msg/*.cxx event/*.cxx device/*.cxx camac/*.cxx caen/*.cxx
   cernlib/*.cxx root/*.cxx common/*.cxx i18n/*.cxx
   LINK_LIBRARIES ${Qt45_LIBRARIES} )
set_target_properties( cdacore PROPERTIES
   VERSION 2.0.0
   SOVERSION 2 )

# Decide how to use the SplitBehaviour enum. The Qt::SplitBehavior enum was
# introduced in some 5.X version. So let's only check with Qt5 if it already has
# it. Qt4 will definitely not.
include( CheckCXXSourceCompiles )
set( CMAKE_REQUIRED_INCLUDES ${Qt5Core_INCLUDE_DIRS} )
set( CMAKE_REQUIRED_FLAGS ${CMAKE_CXX_COMPILE_OPTIONS_PIC} )
set( CMAKE_REQUIRED_DEFINITIONS ${Qt5Core_DEFINITIONS} -DQT_NO_VERSION_TAGGING )
check_cxx_source_compiles( "#include <QString>
   int main() {
      Qt::SplitBehavior sb = Qt::KeepEmptyParts;
      return 0; }"
   CDA_HAVE_QT_SPLITBEHAVIOR )
target_compile_definitions( cdacore PUBLIC
   $<$<BOOL:${CDA_HAVE_QT_SPLITBEHAVIOR}>:CDA_HAVE_QT_SPLITBEHAVIOR> )
unset( CMAKE_REQUIRED_INCLUDES )
unset( CMAKE_REQUIRED_FLAGS )
unset( CMAKE_REQUIRED_DEFINITIONS )

# Use ROOT if it was found:
if( ROOT_FOUND AND CDA_USE_ROOT )
  target_compile_definitions( cdacore PUBLIC HAVE_ROOT_LIBS )
  target_include_directories( cdacore PUBLIC ${ROOT_INCLUDE_DIRS} )
  target_link_libraries( cdacore ${ROOT_LIBRARIES} )
  list( APPEND CMAKE_INSTALL_RPATH ${ROOT_LIBRARY_DIRS} )
  set( CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} PARENT_SCOPE )
  # Remember that ROOT is being used for the build:
  set( CDA_USING_ROOT TRUE CACHE INTERNAL
       "Flag showing that ROOT is used in the build" )
endif()

# Check if the cxxabi.h header file is available. (It's used by the
# ROOT interface code.)
include( CheckIncludeFileCXX )
check_include_file_cxx( cxxabi.h HAVE_CXXABI_H )
if( HAVE_CXXABI_H )
   target_compile_definitions( cdacore PUBLIC HAVE_CXXABI_H )
endif()

# Use CERNLIB if it was found:
if( CERNLIB_FOUND AND CDA_USE_CERNLIB )
  target_compile_definitions( cdacore PRIVATE HAVE_CERNLIB )
  if( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
      ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
    target_compile_definitions( cdacore PRIVATE gFortran )
  elseif( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel" )
    target_compile_definitions( cdacore PRIVATE INTEL_COMPILER )
  else()
    message( WARNING "Compiler not supported for using CERNLIB" )
  endif()
  target_include_directories( cdacore PRIVATE ${CERNLIB_INCLUDE_DIRS} )
  target_link_libraries( cdacore ${CERNLIB_LIBRARIES} )
  list( APPEND CMAKE_INSTALL_RPATH ${CERNLIB_LIBRARY_DIRS} )
  set( CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} PARENT_SCOPE )
  # Remember that CERNLIB is being used for the build:
  set( CDA_USING_CERNLIB TRUE CACHE INTERNAL
       "Flag showing that CERNLIB is used in the build" )
endif()

# Use the CAMAC cc32 library if it was found:
if( CC32_FOUND AND CDA_USE_CC32 )
  target_compile_definitions( cdacore PUBLIC HAVE_CAMAC_LIB )
  target_include_directories( cdacore PUBLIC ${CC32_INCLUDE_DIRS} )
  target_link_libraries( cdacore ${CC32_LIBRARIES} )
endif()

# Use the CAEN digitizer libraries if they were found:
if( CAENDIGITIZER_FOUND AND CDA_USE_CAENDIGITIZER )
  target_compile_definitions( cdacore PRIVATE HAVE_CAEN_DIGITIZER_LIBS
    HAVE_CAEN_COMM_LIBS HAVE_CAEN_VME_LIBS )
  target_include_directories( cdacore PRIVATE ${CAENDIGITIZER_INCLUDE_DIRS} )
  target_link_libraries( cdacore ${CAENDIGITIZER_LIBRARIES} )
endif()

# Use the CAEN QTP libraries if they were found:
if( CAENQTPSDK_FOUND AND CDA_USE_CAENQTPSDK )
  target_compile_definitions( cdacore PRIVATE HAVE_CAEN_QTP_LIBS
    HAVE_CAEN_VME_LIBS )
  target_include_directories( cdacore PRIVATE ${CAENQTPSDK_INCLUDE_DIRS} )
  target_link_libraries( cdacore ${CAENQTPSDK_LIBRARIES} )
endif()

# Generate the CDAVersion.h header:
string( TIMESTAMP BUILD_DATE "%Y-%m-%dT%H%M" )
execute_process( COMMAND git log -1 --format=%h
   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
   RESULT_VARIABLE _gitReturn
   OUTPUT_VARIABLE _gitHash
   OUTPUT_STRIP_TRAILING_WHITESPACE
   ERROR_QUIET )
if( "${_gitReturn}" EQUAL 0 )
   set( GIT_HASH "${_gitHash}" )
else()
   set( GIT_HASH "Unknown" )
endif()
set( _versionDir ${CMAKE_CURRENT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/version )
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/common/CDAVersion.h.in
   ${_versionDir}/CDAVersion.h @ONLY )
target_include_directories( cdacore PUBLIC $<BUILD_INTERFACE:${_versionDir}> )
unset( BUILD_DATE )
unset( _gitReturn )
unset( _gitHash )
unset( GIT_HASH )
unset( _versionDir )
