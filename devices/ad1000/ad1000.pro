# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This is the project file for compiling the ad1000          #
# device plugin.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../devices.pri)

# This will be a library with the name "ad1000":
VERSION  = 1.0.0
TARGET   = ad1000

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/ad1000_hu.ts
