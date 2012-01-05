# Dear emacs, this is -*- fundamental -*- text
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
include(../plugin_app.pri)

# This will be an application with the name "cda-glomem-writer":
TARGET   = cda-glomem-writer

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-glomem-writer_hu.ts
