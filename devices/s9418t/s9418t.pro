# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the project file for compiling the s9418t          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "s9418t":
VERSION  = 1.0.0
TARGET   = s9418t

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/s9418t_hu.ts

mac {
   message(*)
   message(* Configuring to build the \"s9418t\" plugin on Mac OS X)
   message(*)

   # The plugin has to be linked against the cdacore framework:
   INCLUDEPATH    += ../../core
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

unix:!mac {
   message(*)
   message(* Configuring to build the \"s9418t\" plugin on Linux)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = -L../../lib -lcdacore
}

win32 {
   message(*)
   message(* Configuring to build the \"s9418t\" plugin on Windows)
   message(*)

   # The plugin has to be linked against the cdacore library:
   INCLUDEPATH += ../../core
   LIBS         = ../../lib/libcdacore1.a
}
