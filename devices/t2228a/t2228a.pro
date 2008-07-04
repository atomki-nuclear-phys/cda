# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the t2228a          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)

# This will be a library with the name "example":
TEMPLATE = lib
VERSION  = 0.0.1
TARGET   = t2228a

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The library uses the QtCore, QtGui and QtXml libraries:
CONFIG += qt debug warn_on plugin static
QT      = core gui xml

# Link the plugin against CERNLIB:
DEFINES     += LINUX f2cFortran
LIBS        += -L$$CERNLIB_PATH/lib -lpacklib
INCLUDEPATH += $$CERNLIB_PATH/include

# The place to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../dev

mac {
   message(*)
   message(* Configuring to build the \"t2228a\" plugin on Mac OS X)
   message(*)
   

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore

   # On Mac OS X CERNLIB is compiled using gfortran:
   LIBS += -lgfortran
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"t2228a\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = -L../../lib -lcdacore

   # On Linux CERNLIB is compiled using g77:
   LIBS += -lg2c
}
