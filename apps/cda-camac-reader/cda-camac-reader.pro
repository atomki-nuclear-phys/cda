# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the cda-camac-reader            #
# application.                                               #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../console_app.pri)

# This will be an application with the name "cda-camac-reader":
TARGET   = cda-camac-reader

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cda-camac-reader\" application on Mac OS X)
   message(*)

   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore
}

#
# These are the specific configuration options for compiling the code
# on Linux.
#
unix:!mac {
   message(*)
   message(* Configuring to build the \"cda-camac-reader\" application on Linux)
   message(*)

   INCLUDEPATH += ../../core
   LIBS        += -L../../lib -lcdacore
}
