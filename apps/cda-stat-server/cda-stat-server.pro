# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the cda-stat-server used to     #
# present the status of a complex DAQ system.                #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)

# This will be an application with the name "cda-stat-server":
TARGET = cda-stat-server

# These are the header and source files:
HEADERS = *.h
SOURCES = *.cxx

# The application uses some Qt resources:
RESOURCES = cda-stat-server.qrc

# The translation file:
TRANSLATIONS = ../../trans/cda-stat-server_hu.ts

mac {
   ICON = ../../images/cda-stat-server.icns
}
