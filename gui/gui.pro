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
TARGET   = cdagui

# These are the header and source files:
HEADERS = $$files(msg/*.h) $$files(device/*.h) $$files(simple_daq/*.h) \
          $$files(stat/*.h) $$files(common/*.h)
SOURCES = $$files(msg/*.cxx) $$files(device/*.cxx) $$files(simple_daq/*.cxx) \
          $$files(stat/*.cxx) $$files(common/*.cxx)
TRANSLATIONS = ../trans/cdagui_hu.ts

# The library uses the QtCore, QtNetwork, QtGui and QtXml libraries:
CONFIG += qt
QT      = core network gui widgets xml

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
   MSG_HEADERS.version = Versions
   MSG_HEADERS.files = $$files(msg/*.h)
   MSG_HEADERS.path = Headers/msg
   QMAKE_BUNDLE_DATA += MSG_HEADERS

   DEVICE_HEADERS.version = Versions
   DEVICE_HEADERS.files = $$files(device/*.h)
   DEVICE_HEADERS.path = Headers/device
   QMAKE_BUNDLE_DATA += DEVICE_HEADERS

   SIMPLE_DAQ_HEADERS.version = Versions
   SIMPLE_DAQ_HEADERS.files = $$files(simple_daq/*.h)
   SIMPLE_DAQ_HEADERS.path = Headers/simple_daq
   QMAKE_BUNDLE_DATA += SIMPLE_DAQ_HEADERS

   STAT_HEADERS.version = Versions
   STAT_HEADERS.files = $$files(stat/*.h)
   STAT_HEADERS.path = Headers/stat
   QMAKE_BUNDLE_DATA += STAT_HEADERS

   COMMON_HEADERS.version = Versions
   COMMON_HEADERS.files = $$files(common/*.h)
   COMMON_HEADERS.path = Headers/common
   QMAKE_BUNDLE_DATA += COMMON_HEADERS

   DUMMY_HEADERS.version = Versions
   DUMMY_HEADERS.files =
   DUMMY_HEADERS.path = Headers
   QMAKE_BUNDLE_DATA += DUMMY_HEADERS

   # This framework has to be linked against the cdacore framework:
   QMAKE_CXXFLAGS += -F$$OUT_PWD/../lib
   LIBS           += -F$$OUT_PWD/../lib -framework cdacore -framework cdadaq

   # Here we do the same trick as with cdacore. See core/core.pro for details.
   DESTDIR                     = /
   QMAKE_FRAMEWORK_BUNDLE_NAME = $$OUT_PWD/../lib/cdagui
}

#
# These are the configuration options for compiling the code on Linux.
# Way simpler than the Mac OS X ones...
#
unix:!mac {
   # Create a shared library and link it against the cdacore library:
   CONFIG      += shared
   INCLUDEPATH += ../core ../daq
   LIBS        += -L../lib -lcdacore -lcdadaq

   # Place the library in ../lib:
   DESTDIR      = ../lib
}

#
# These are the configuration options for compiling the code on Windows:
#
win32 {
   # Create a static library and link it against the cdacore library:
   CONFIG      += static
   INCLUDEPATH += ../core ../daq
   LIBS        += -L../lib -lcdadaq -lcdacore

   # Place the library in ../bin:
   DESTDIR      = ../lib
}
