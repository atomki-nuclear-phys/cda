# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the cda-root-writer             #
# application.                                               #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../plugin_app.pri)
include(../console_app.pri)

# This will be an application with the name "cda-root-writer":
TARGET   = cda-root-writer

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-root-writer_hu.ts
