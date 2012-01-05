# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file compiles the cda-qt-monitoring           #
# application.                                               #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../../cda_common.pri)
include(../gui_app.pri)
include(../plugin_app.pri)

# This will be an application with the name "cda-qt-monitoring":
TARGET = cda-qt-monitoring

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx

# The application uses some resources:
RESOURCES = cda-qt-monitoring.qrc

# The translation file for the application:
TRANSLATIONS = ../../trans/cda-qt-monitoring_hu.ts

mac {
   ICON = ../../images/cda-qt-monitoring.icns
}
