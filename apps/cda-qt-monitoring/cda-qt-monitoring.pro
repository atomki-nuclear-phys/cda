# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the cda-qt-monitoring application.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)
include(../plugin_app.pri)

# This will be an application with the name "cda-qt-monitoring":
TARGET = cda-qt-monitoring

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx

# The application uses some resources:
RESOURCES = cda-qt-monitoring.qrc

# The translation file for the application:
TRANSLATIONS = ../../trans/cda-qt-monitoring_hu.ts

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cda-qt-monitoring\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq -framework cdagui

#   ICON = ../../images/cda-config-editor.icns
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-qt-monitoring\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core ../../gui
   LIBS        += -L../../lib -lcdacore -lcdadaq -lcdagui
}

#
# These are the specific configuration options for compiling the code
# on Windows.
#
win32 {
   message(*)
   message(* Configuring to build the \"cda-qt-monitoring\" application on Windows)
   message(*)

   INCLUDEPATH += ../../core ../../gui
   LIBS        += -L../../lib -lcdagui -lcdadaq -lcdagui
}
