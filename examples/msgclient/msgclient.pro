# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the msgclient example.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)

# This will be an application with the name "msgclient":
TEMPLATE = app
TARGET   = msgclient

# These are the header and source files:
HEADERS =
SOURCES = msgclient.cxx

# The application uses the QtCore and QtNetwork libraries:
CONFIG = qt debug warn_on
QT     = core network

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
   message(* Configuring to build the \"msgclient\" example on Mac OS X)
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
   message(* Configuring to build the \"msgclient\" example on Linux)
   message(*)

   INCLUDEPATH += ../../core
   LIBS        += -L../../lib -lcdacore
}
