# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This is the project file for compiling the t4300b          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "t4300b":
VERSION  = 1.0.0
TARGET   = t4300b

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/t4300b_hu.ts
