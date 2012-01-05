# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the cda-camac-daq front-end     #
# for running CDA.                                           #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)

# This will be an application with the name "cda-camac-daq":
TARGET = cda-camac-daq

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx
TRANSLATIONS = ../../trans/cda-camac-daq_hu.ts

# The application uses some Qt resources:
RESOURCES = cda-camac-daq.qrc

mac {
   ICON = ../../images/cda-daq.icns
}
