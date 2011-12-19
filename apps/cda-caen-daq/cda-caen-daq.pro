# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the cda-caen-daq front-end      #
# for running CDA.                                           #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)

# This will be an application with the name "cda-caen-daq":
TARGET = cda-caen-daq

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-caen-daq_hu.ts

# The application uses some Qt resources:
RESOURCES = cda-caen-daq.qrc

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cda-caen-daq\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq -framework cdagui

   ICON = ../../images/caen.icns
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-caen-daq\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core ../../gui ../../daq
   LIBS        += -L../../lib -lcdacore -lcdadaq -lcdagui
}

#
# These are the specific configuration options for compiling the code
# on Windows.
#
win32 {
   message(*)
   message(* Configuring to build the \"cda-caen-daq\" application on Windows)
   message(*)

   INCLUDEPATH += ../../core ../../gui ../../daq
   LIBS        += -L../../lib -lcdagui -lcdadaq -lcdacore
}
