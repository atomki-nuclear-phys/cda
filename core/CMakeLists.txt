# $Id$
#
# Configuration for building the CDA core library.
#

# Greet the user:
message( STATUS "Configuring the build of cdacore" )

# Find Qt:
find_package( Qt5 QUIET COMPONENTS Core Network Gui Widgets )
if( Qt5_FOUND AND NOT CDA_USE_QT4 )
  message( STATUS "Using Qt5 for the build" )
  set( qt_libraries Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets )
else()
  find_package( Qt4 QUIET COMPONENTS QtCore QtNetwork QtGui )
  if( NOT QT_FOUND )
    message( SEND_ERROR "No working version of Qt was found" )
    return()
  endif()
  message( STATUS "Using Qt4 for the build" )
  set( qt_libraries Qt4::QtCore Qt4::QtNetwork Qt4::QtGui )
endif()

# Optional libraries to use for the build:
find_package( ROOT )
find_package( CERNLIB COMPONENTS packlib )

# Optional device libraries to use for the build:
find_package( cc32 )
find_package( CAENDigitizer )
find_package( CAENqtpSDK )

# Collect the source files of the library:
file( GLOB headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   fifo/*.h msg/*.h event/*.h device/*.h device/*.icc
   camac/*.h caen/*.h cernlib/*.h root/*.h root/*.icc
   common/*.h i18n/*.h )
file( GLOB sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
   fifo/*.cxx msg/*.cxx event/*.cxx device/*.cxx
   camac/*.cxx caen/*.cxx cernlib/*.cxx root/*.cxx
   common/*.cxx i18n/*.cxx )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Set up the hungarian translations:
set( qm_files )
set( ts_file ${CMAKE_SOURCE_DIR}/trans/cdacore_hu.ts )
set_source_files_properties( ${ts_file} PROPERTIES OUTPUT_LOCATION
   ${CMAKE_BINARY_DIR}/trans )
if( Qt5_FOUND AND NOT CDA_USE_QT4 )
   find_package( Qt5LinguistTools )
   if( Qt5LinguistTools_FOUND )
      qt5_create_translation( qm_files ${sources} ${ts_file} )
   endif()
else()
   qt4_create_translation( qm_files ${sources} ${ts_file} )
endif()
install( FILES ${qm_files} DESTINATION trans )

# Build the cdacore library:
add_library( cdacore STATIC ${headers} ${sources} ${qm_files} )
target_include_directories( cdacore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( cdacore PUBLIC ${qt_libraries} )
set_target_properties( cdacore PROPERTIES
  VERSION 2.0.0 SOVERSION 2 )
target_compile_definitions( cdacore PRIVATE CDACORE_LIBRARY )
set_property( TARGET cdacore PROPERTY FOLDER core )

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

# Use ROOT if it was found:
if( ROOT_FOUND AND CDA_USE_ROOT )
  target_compile_definitions( cdacore PUBLIC HAVE_ROOT_LIBS )
  target_include_directories( cdacore PUBLIC ${ROOT_INCLUDE_DIRS} )
  target_link_libraries( cdacore PUBLIC ${ROOT_LIBRARIES} )
  list( APPEND CMAKE_INSTALL_RPATH ${ROOT_LIBRARY_DIRS} )
  set( CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} PARENT_SCOPE )
endif()

# Check if the cxxabi.h header file is available. (It's used by the
# ROOT interface code.)
include( CheckIncludeFiles )
check_include_files( cxxabi.h HAVE_CXXABI_H )
if( HAVE_CXX_ABI )
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
  target_link_libraries( cdacore PUBLIC ${CERNLIB_LIBRARIES} )
  list( APPEND CMAKE_INSTALL_RPATH ${CERNLIB_LIBRARY_DIRS} )
  set( CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_RPATH} PARENT_SCOPE )
endif()

# Use the CAMAC cc32 library if it was found:
if( CC32_FOUND AND CDA_USE_CC32 )
  target_compile_definitions( cdacore PUBLIC HAVE_CAMAC_LIB )
  target_include_directories( cdacore PUBLIC ${CC32_INCLUDE_DIRS} )
  target_link_libraries( cdacore PRIVATE ${CC32_LIBRARIES} )
endif()

# Use the CAEN digitizer libraries if they were found:
if( CAENDIGITIZER_FOUND AND CDA_USE_CAENDIGITIZER )
  target_compile_definitions( cdacore PRIVATE HAVE_CAEN_DIGITIZER_LIBS
    HAVE_CAEN_COMM_LIBS HAVE_CAEN_VME_LIBS )
  target_include_directories( cdacore PRIVATE ${CAENDIGITIZER_INCLUDE_DIRS} )
  target_link_libraries( cdacore PRIVATE ${CAENDIGITIZER_LIBRARIES} )
endif()

# Use the CAEN QTP libraries if they were found:
if( CAENQTPSDK_FOUND AND CDA_USE_CAENQTPSDK )
  target_compile_definitions( cdacore PRIVATE HAVE_CAEN_QTP_LIBS
    HAVE_CAEN_VME_LIBS )
  target_include_directories( cdacore PRIVATE ${CAENQTPSDK_INCLUDE_DIRS} )
  target_link_libraries( cdacore PRIVATE ${CAENQTPSDK_LIBRARIES} )
endif()
