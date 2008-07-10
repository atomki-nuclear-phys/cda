# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the cda-glomem-writer           #
# application.                                               #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)

# This will be an application with the name "cda-camacreader":
TEMPLATE = app
TARGET   = cda-glomem-writer

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses the QtCore and QtNetwork libraries:
CONFIG = qt debug warn_on
QT     = core network xml gui

# Link with the static plugins:
LIBS += -L../../dev -lt2228a

# Link the application against CERNLIB:
DEFINES     += LINUX f2cFortran
LIBS        += -L$$CERNLIB_PATH/lib
INCLUDEPATH += $$CERNLIB_PATH/include

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
   message(* Configuring to build the \"cda-glomem-writer\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore

   # On Mac OS X CERNLIB is compiled using gfortran:
   LIBS += -lpacklib -lgfortran
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-glomem-writer\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core
   LIBS        += -L../../lib -lcdacore

   # On Linux CERNLIB is compiled using g77:
   LIBS += -lpacklib_noshift -lg2c
}
