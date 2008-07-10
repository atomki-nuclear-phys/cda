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

# This will be an application with the name "cda-config-editor":
TEMPLATE = app
TARGET   = cda-config-editor

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses the QtCore, QtNetwork and QtGui libraries,
# and it uses some built in resources.
CONFIG   += qt debug warn_on
QT        = core gui network xml
RESOURCES = cda-config-editor.qrc

# Link with the static plugins:
LIBS += -L../../dev -lt2228a

# Link the application against CERNLIB:
LIBS += -L$$CERNLIB_PATH/lib

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
   message(* Configuring to build the \"cda-config-editor\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdagui

   # On Mac OS X CERNLIB is compiled using gfortran:
   LIBS += -lpacklib -lgfortran

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

   # On Linux CERNLIB is compiled using g77:
   LIBS += -lpacklib_noshift -lg2c
}
