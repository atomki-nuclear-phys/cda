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
include(../plugin_app.pri)
include(../gui_app.pri)

# This will be an application with the name "cda-config-editor":
TARGET = cda-config-editor

# These are the header and source files:
HEADERS = *.h ../win32/plugins.h
SOURCES = *.cxx

# The application uses some resources:
RESOURCES += cda-config-editor.qrc

# The translation file for the application:
TRANSLATIONS = ../../trans/cda-config-editor_hu.ts

mac {
   ICON = ../../images/cda-config-editor.icns
}
