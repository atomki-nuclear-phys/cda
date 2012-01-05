# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the msgserver example.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)

# This will be an application with the name "cda-msgserver":
TARGET = cda-msgserver

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses some resources:
RESOURCES = cda-msgserver.qrc

# The translations:
TRANSLATIONS = ../../trans/cda-msgserver_hu.ts

mac {
   ICON = ../../images/cda-msgserver.icns
}
