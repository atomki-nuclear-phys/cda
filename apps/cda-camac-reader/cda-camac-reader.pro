# Dear emacs, this is -*- fundamental -*- text
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
include(../plugin_app.pri)
include(../console_app.pri)

# This will be an application with the name "cda-camac-reader":
TARGET   = cda-camac-reader

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-camac-reader_hu.ts
