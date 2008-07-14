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

# This will be an application with the name "cda-glomem-writer":
TARGET   = cda-glomem-writer

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

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
