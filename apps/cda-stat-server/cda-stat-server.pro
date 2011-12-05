# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the cda-stat-server used to     #
# present the status of a complex DAQ system.                #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)

# This will be an application with the name "cda-stat-server":
TARGET = cda-stat-server

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses some Qt resources:
RESOURCES = cda-stat-server.qrc

# The translation file:
TRANSLATIONS = ../../trans/cda-stat-server_hu.ts

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cda-stat-server\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq -framework cdagui

   ICON = ../../images/cda-stat-server.icns
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-stat-server\" application on Linux)
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
   message(* Configuring to build the \"cda-stat-server\" application on Windows)
   message(*)

   INCLUDEPATH += ../../core ../../gui ../../daq
   LIBS        += -L../../lib -lcdagui -lcdadaq -lcdacore
}
