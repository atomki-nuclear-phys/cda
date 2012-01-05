# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This is the project file for compiling the t2228a          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "t2228a":
VERSION  = 1.0.0
TARGET   = t2228a

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/t2228a_hu.ts
