# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the pluginloader example.       #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common checks:
include(../../cda_checks.pri)

# This will be an application with the name "pluginloader":
TEMPLATE = app
TARGET   = pluginloader

# These are the header and source files:
HEADERS =
SOURCES = pluginloader.cxx

# The application uses the QtCore, QtNetwork, QtGui and QtXml libraries:
CONFIG += qt debug warn_on
QT      = core network gui xml

# The places to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../bin

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"pluginloader\" example on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"pluginloader\" example on Linux)
   message(*)

   INCLUDEPATH += ../../core
   LIBS        += -L../../lib -lcdacore
}
