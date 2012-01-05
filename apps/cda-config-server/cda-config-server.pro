# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the cda-config-server           #
# application.                                               #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../console_app.pri)
include(../plugin_app.pri)

# This will be an application with the name "cda-config-server":
TARGET = cda-config-server

# These are the header and source files:
HEADERS = ../win32/plugins.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-config-server_hu.ts
