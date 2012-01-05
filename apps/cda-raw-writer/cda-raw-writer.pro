# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the cda-raw-writer              #
# application.                                               #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../console_app.pri)
include(../plugin_app.pri)

# This will be an application with the name "cda-hbook-writer":
TARGET   = cda-raw-writer

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-raw-writer_hu.ts
