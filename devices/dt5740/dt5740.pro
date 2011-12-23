# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the dt5740          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "dt5740":
VERSION  = 1.0.0
TARGET   = dt5740

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
RESOURCES = dt5740.qrc
TRANSLATIONS = ../../trans/dt5740_hu.ts

mac {
   message(*)
   message(* Configuring to build the \"dt5740\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"dt5740\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core ../../daq
   LIBS         = -L../../lib -lcdacore -lcdadaq
}

win32 {
   message(*)
   message(* Configuring to build the \"dt5740\" plugin on Windows)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core ../../daq
   LIBS         = ../../lib/libcdacore1.a ../../lib/libcdadaq1.a
}
