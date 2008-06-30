# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the example CAMAC   #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common checks:
include(../../cda_checks.pri)

# This will be a library with the name "example":
TEMPLATE = lib
VERSION  = 0.0.1
TARGET   = example

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The library uses the QtCore, QtGui and QtXml libraries:
CONFIG = qt debug warn_on plugin
QT     = core gui xml

# The place to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../dev

mac {
   message(*)
   message(* Configuring to build the \"example\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"example\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = -L../../lib -lcdacore
}
