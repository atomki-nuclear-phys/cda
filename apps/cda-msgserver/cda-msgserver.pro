# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the msgserver example.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)

# This will be an application with the name "cda-msgserver":
TEMPLATE = app
TARGET   = cda-msgserver

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses the QtCore, QtNetwork and QtGui libraries,
# and it uses some built in resources.
CONFIG   += qt debug warn_on
QT        = core gui network
RESOURCES = cda-msgserver.qrc

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
   message(* Configuring to build the \"cda-msgserver\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdagui

   ICON = ../../images/cda-msgserver.icns
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-msgserver\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core ../../gui
   LIBS        += -L../../lib -lcdacore -lcdagui
}
