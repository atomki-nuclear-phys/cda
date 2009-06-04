# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the confserver example.         #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)

# This will be an application with the name "fiforeader":
TEMPLATE = app
TARGET   = confserver

# These are the header and source files:
HEADERS =
SOURCES = confserver.cxx

# The application uses the QtCore and QtNetwork libraries:
CONFIG = qt debug warn_on
QT     = core xml network

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
   message(* Configuring to build the \"confserver\" example on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"confserver\" example on Linux)
   message(*)

   INCLUDEPATH += ../../core ../../daq
   LIBS        += -L../../lib -lcdacore -l cdadaq
}
