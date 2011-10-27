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
include(../gui_app.pri)

# This will be an application with the name "cda-config-editor":
TARGET = cda-config-editor

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses some resources:
RESOURCES = cda-config-editor.qrc

# The translation file for the application:
TRANSLATIONS = ../../trans/cda-config-editor_hu.ts

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cda-config-editor\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdagui

   ICON = ../../images/cda-config-editor.icns
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-config-editor\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core ../../gui
   LIBS        += -L../../lib -lcdacore -lcdagui
}
