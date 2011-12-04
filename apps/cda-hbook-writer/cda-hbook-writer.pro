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
include(../console_app.pri)

# This will be an application with the name "cda-hbook-writer":
TARGET   = cda-hbook-writer

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-hbook-writer_hu.ts

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cda-hbook-writer\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-hbook-writer\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core ../../daq
   LIBS        += -L../../lib -lcdacore -lcdadaq
}

#
# These are the specific configuration options for compiling the code
# on Windows.
#
win32 {
   message(*)
   message(* Configuring to build the \"cda-hbook-writer\" application on Windows)
   message(*)

   INCLUDEPATH += ../../core ../../daq
   LIBS        += -L../../bin -lcdacore0 -lcdadaq0 -L../../dev -lad413a -lad1000 \
                  -lad2249a -lt2228a -lt4300b -ls9418t
}
