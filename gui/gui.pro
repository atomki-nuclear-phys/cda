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
VERSION  = 0.9.0
TARGET   = cdagui

# These are the header and source files:
HEADERS = $$system(ls msg/*.h) $$system(ls device/*.h) $$system(ls simple_daq/*.h) \
          $$system(ls stat/*.h)
SOURCES = $$system(ls msg/*.cxx) $$system(ls device/*.cxx) $$system(ls simple_daq/*.cxx) \
          $$system(ls stat/*.cxx)
TRANSLATIONS = cdagui_hu.ts

# The library uses the QtCore, QtNetwork, QtGui and QtXml libraries:
CONFIG += qt
QT      = core network gui xml

# The place to put the intermediate build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cdagui\" framework on Mac OS X)
   message(*)

   # We create a library bundle and create a file storing the dependencies
   # of this bundle:
   CONFIG += lib_bundle create_prl link_prl

   # The headers are put in the framework in the same way as in the case of
   # the cdacore framework. See core/core.pro for details.
   MSG_HEADERS.version = Versions
   MSG_HEADERS.files = $$system(ls msg/*.h)
   MSG_HEADERS.path = Headers/msg
   QMAKE_BUNDLE_DATA += MSG_HEADERS

   DEVICE_HEADERS.version = Versions
   DEVICE_HEADERS.files = $$system(ls device/*.h)
   DEVICE_HEADERS.path = Headers/device
   QMAKE_BUNDLE_DATA += DEVICE_HEADERS

   SIMPLE_DAQ_HEADERS.version = Versions
   SIMPLE_DAQ_HEADERS.files = $$system(ls simple_daq/*.h)
   SIMPLE_DAQ_HEADERS.path = Headers/simple_daq
   QMAKE_BUNDLE_DATA += SIMPLE_DAQ_HEADERS

   STAT_HEADERS.version = Versions
   STAT_HEADERS.files = $$system(ls stat/*.h)
   STAT_HEADERS.path = Headers/stat
   QMAKE_BUNDLE_DATA += STAT_HEADERS

   DUMMY_HEADERS.version = Versions
   DUMMY_HEADERS.files =
   DUMMY_HEADERS.path = Headers
   QMAKE_BUNDLE_DATA += DUMMY_HEADERS

   # This framework has to be linked against the cdacore framework:
   QMAKE_CXXFLAGS += -F../lib
   LIBS           += -F../lib -framework cdacore -framework cdadaq

   # Here we do the same trick as with cdacore. See core/core.pro for details.
   DESTDIR                     = /
   QMAKE_FRAMEWORK_BUNDLE_NAME = $$CDASYS/lib/cdagui
}

#
# These are the configuration options for compiling the code on Linux.
# Way simpler than the Mac OS X ones...
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cdagui\" shared library on Linux)
   message(*)

   # Create a shared library and link it against the cdacore library:
   CONFIG      += shared
   INCLUDEPATH += ../core ../daq
   LIBS        += -L../lib -lcdacore -l cdadaq

   # Place the library in ../lib:
   DESTDIR      = ../lib
}
