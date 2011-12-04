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
include(../devices.pri)

# This will be a library with the name "t2228a":
VERSION  = 1.0.0
TARGET   = t2228a

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/t2228a_hu.ts

mac {
   message(*)
   message(* Configuring to build the \"t2228a\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"t2228a\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = -L../../lib -lcdacore
}

win32 {
   message(*)
   message(* Configuring to build the \"t2228a\" plugin on Windows)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = ../../bin/libcdacore0.a
}
