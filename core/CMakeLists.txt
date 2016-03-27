# $Id$
#
# Configuration for building the CDA core library.
#

# Greet the user:
message( STATUS "Configuring the build of cdacore" )

# Find Qt:
find_package( Qt5 COMPONENTS Core Network Gui Widgets )
if( Qt5_FOUND )
  message( STATUS "Using Qt5 for the build" )
  set( qt_libraries Qt5::Core Qt5::Network Qt5::Gui Qt5::Widgets )
else()
  find_package( Qt4 COMPONENTS QtCore QtNetwork QtGui )
  if( NOT Qt4_FOUND )
    message( ERROR "No working version of Qt was found" )
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
file( GLOB headers fifo/*.h msg/*.h event/*.h device/*.h device/*.icc
  camac/*.h caen/*.h vme/*.h cernlib/*.h root/*.h root/*.icc
  common/*.h i18n/*.h )
file( GLOB sources fifo/*.cxx msg/*.cxx event/*.cxx device/*.cxx
  camac/*.cxx caen/*.cxx vme/*.cxx cernlib/*.cxx root/*.cxx
  common/*.cxx i18n/*.cxx )

# Generate MOC files automatically:
set( CMAKE_AUTOMOC TRUE )

# Build the cdacore library:
add_library( cdacore SHARED ${headers} ${sources} )
target_include_directories( cdacore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( cdacore PUBLIC ${qt_libraries} )
set_target_properties( cdacore PROPERTIES
  VERSION 2.0.0 SOVERSION 2 )
target_compile_definitions( cdacore PRIVATE CDACORE_LIBRARY )

# Use ROOT if it was found:
if( ROOT_FOUND AND CDA_USE_ROOT )
  target_compile_definitions( cdacore PUBLIC HAVE_ROOT_LIBS )
  target_include_directories( cdacore PUBLIC ${ROOT_INCLUDE_DIRS} )
  target_link_libraries( cdacore PUBLIC ${ROOT_LIBRARIES} )
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
endif()

# Use the CAMAC cc32 library if it was found:
if( CC32_FOUND AND CDA_USE_CC32 )
  target_compile_definitions( cdacore PUBLIC HAVE_CAMAC_LIB )
  target_include_directories( cdacore PUBLIC ${CC32_INCLUDE_DIRS} )
  target_link_libraries( cdacore PRIVATE ${CC32_LIBRARIES} )
endif()

# Use the CAEN digitizer libraries if they were found:
if( CAENDIGITIZER_FOUND AND CDA_USE_CAENDIGITIZER )
  target_compile_definitions( cdacore PRIVATE HAVE_CAEN_DIGITIZER_LIBS )
  target_include_directories( cdacore PRIVATE ${CAENDIGITIZER_INCLUDE_DIRS} )
  target_link_libraries( cdacore PRIVATE ${CAENDIGITIZER_LIBRARIES} )
endif()

# Use the CAEN QTP libraries if they were found:
if( CAENQTPSDK_FOUND AND CDA_USE_CAENQTPSDK )
  target_compile_definitions( cdacore PRIVATE HAVE_CAEN_QTP_LIBS )
  target_include_directories( cdacore PRIVATE ${CAENQTPSDK_INCLUDE_DIRS} )
  target_link_libraries( cdacore PRIVATE ${CAENQTPSDK_LIBRARIES} )
endif()
