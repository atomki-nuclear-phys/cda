# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the gui library of CDA.         #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../cda_common.pri)

# This will be a library with the name "cdagui":
TEMPLATE = lib
VERSION  = 1.1.0
TARGET   = cdadaq

# These are the header and source files:
HEADERS = $$files(common/*.h) $$files(stat/*.h) \
          $$files(config/*.h) $$files(moni/*.h)
SOURCES = $$files(common/*.cxx) $$files(stat/*.cxx) \
          $$files(config/*.cxx) $$files(moni/*.cxx)
TRANSLATIONS = ../trans/cdadaq_hu.ts

# The library uses the QtCore library:
CONFIG += qt
QT      = core xml network gui widgets

# The place to put the intermediate build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   # We create a library bundle and create a file storing the dependencies
   # of this bundle:
   CONFIG += lib_bundle create_prl link_prl

   # The headers are put in the framework in the same way as in the case of
   # the cdacore framework. See core/core.pro for details.
   COMMON_HEADERS.version = Versions
   COMMON_HEADERS.files = $$files(common/*.h)
   COMMON_HEADERS.path = Headers/common
   QMAKE_BUNDLE_DATA += COMMON_HEADERS

   STAT_HEADERS.version = Versions
   STAT_HEADERS.files = $$files(stat/*.h)
   STAT_HEADERS.path = Headers/stat
   QMAKE_BUNDLE_DATA += STAT_HEADERS

   CONFIG_HEADERS.version = Versions
   CONFIG_HEADERS.files = config/ConfServer.h config/ConfReader.h
   CONFIG_HEADERS.path = Headers/config
   QMAKE_BUNDLE_DATA += CONFIG_HEADERS

   MONI_HEADERS.version = Versions
   MONI_HEADERS.files = $$files(moni/*.h)
   MONI_HEADERS.path = Headers/moni
   QMAKE_BUNDLE_DATA += MONI_HEADERS

   DUMMY_HEADERS.version = Versions
   DUMMY_HEADERS.files =
   DUMMY_HEADERS.path = Headers
   QMAKE_BUNDLE_DATA += DUMMY_HEADERS

   # This framework has to be linked against the cdacore framework:
   QMAKE_CXXFLAGS += -F$$CDASYS/lib
   LIBS           += -F$$CDASYS/lib -framework cdacore

   # Here we do the same trick as with cdacore. See core/core.pro for details.
   DESTDIR                     = /
   QMAKE_FRAMEWORK_BUNDLE_NAME = $$CDASYS/lib/cdadaq
}

#
# These are the configuration options for compiling the code on Linux.
# Way simpler than the Mac OS X ones...
#
unix:!mac {
   # Create a shared library and link it against the cdacore library:
   CONFIG      += shared
   INCLUDEPATH += ../core
   LIBS        += -L../lib -lcdacore

   # Place the library in ../lib:
   DESTDIR      = ../lib
}

#
# These are the configuration options for compiling the code on Windows:
#
win32 {
   # Create a static library and link it against the cdacore library:
   CONFIG      += static
   INCLUDEPATH += ../core
   LIBS        += -L../lib -lcdacore

   # Place the library in ../lib:
   DESTDIR      = ../lib
}
