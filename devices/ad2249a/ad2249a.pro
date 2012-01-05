# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the ad2249a         #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "ad2249a":
VERSION  = 1.0.0
TARGET   = ad2249a

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/ad2249a_hu.ts

mac {
   message(*)
   message(* Configuring to build the \"ad2249a\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core ../../daq
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"ad2249a\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core ../../daq
   LIBS         = -L../../lib -lcdacore -lcdadaq
}

win32 {
   message(*)
   message(* Configuring to build the \"ad2249a\" plugin on Windows)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = ../../lib/libcdadaq1.a ../../lib/libcdacore1.a
}
