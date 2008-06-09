
##############################################################
#                                                            #
# This project file compiles the pluginloader example.       #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = app
TARGET   = pluginloader

HEADERS =
SOURCES = pluginloader.cxx

CONFIG      += qt debug warn_on
QT           = core network gui
INCLUDEPATH += ../../core
LIBS         = -L../../lib -lcdacore

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../bin
