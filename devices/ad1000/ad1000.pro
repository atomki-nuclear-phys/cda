# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the ad1000          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "ad1000":
VERSION  = 1.0.0
TARGET   = ad1000

# These are the header and source files:
HEADERS = $$system(ls *.h)
SOURCES = $$system(ls *.cxx)
TRANSLATIONS = ../../dev/ad1000_hu.ts

mac {
   message(*)
   message(* Configuring to build the \"ad1000\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"ad1000\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = -L../../lib -lcdacore
}
