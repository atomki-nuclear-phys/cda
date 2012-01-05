# Dear emacs, this is -*- fundamental -*- text
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
