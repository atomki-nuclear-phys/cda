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
TARGET   = cdadaq

# These are the header and source files:
HEADERS = $$system(ls common/*.h) $$system(ls schema/*.h) $$system(ls stat/*.h)
SOURCES = $$system(ls common/*.cxx) $$system(ls schema/*.cxx) $$system(ls stat/*.cxx)
TRANSLATIONS = cdadaq_hu.ts

# The library uses the QtCore library:
CONFIG += qt
QT      = core xml network

# The place to put the intermediate build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cdadaq\" framework on Mac OS X)
   message(*)

   # We create a library bundle and create a file storing the dependencies
   # of this bundle:
   CONFIG += lib_bundle create_prl link_prl

   # The headers are put in the framework in the same way as in the case of
   # the cdacore framework. See core/core.pro for details.
   COMMON_HEADERS.version = Versions
   COMMON_HEADERS.files = $$system(ls common/*.h)
   COMMON_HEADERS.path = Headers/common
   QMAKE_BUNDLE_DATA += COMMON_HEADERS

   SCHEMA_HEADERS.version = Versions
   SCHEMA_HEADERS.files = $$system(ls schema/*.h)
   SCHEMA_HEADERS.path = Headers/schema
   QMAKE_BUNDLE_DATA += SCHEMA_HEADERS

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
   LIBS           += -F../lib -framework cdacore

   # Here we do the same trick as with cdacore. See core/core.pro for details.
   DESTDIR                     = /
   QMAKE_FRAMEWORK_BUNDLE_NAME = $$CDASYS/lib/cdadaq
}

#
# These are the configuration options for compiling the code on Linux.
# Way simpler than the Mac OS X ones...
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cdadaq\" shared library on Linux)
   message(*)

   # Create a shared library and link it against the cdacore library:
   CONFIG      += shared
   INCLUDEPATH += ../core
   LIBS        += -L../lib -lcdacore

   # Place the library in ../lib:
   DESTDIR      = ../lib
}