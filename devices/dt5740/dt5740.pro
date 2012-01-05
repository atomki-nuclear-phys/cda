# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This is the project file for compiling the dt5740          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "dt5740":
VERSION  = 1.0.0
TARGET   = dt5740

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
RESOURCES = dt5740.qrc
TRANSLATIONS = ../../trans/dt5740_hu.ts
