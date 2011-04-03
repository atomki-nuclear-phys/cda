# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the ad413a         #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "ad413a":
VERSION  = 1.0.0
TARGET   = ad413a

# These are the header and source files:
HEADERS = $$system(ls *.h)
SOURCES = $$system(ls *.cxx)
TRANSLATIONS = ../../trans/ad413a_hu.ts

mac {
   message(*)
   message(* Configuring to build the \"ad413a\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"ad413a\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = -L../../lib -lcdacore
}
