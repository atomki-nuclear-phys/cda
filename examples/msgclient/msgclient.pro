
##############################################################
#                                                            #
# This project file compiles the msgclient example.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = app
TARGET   = msgclient

HEADERS =
SOURCES = msgclient.cxx

CONFIG       = qt debug warn_on
QT           = core network
INCLUDEPATH += ../../core
LIBS         = -L../../lib -lcdacore

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../bin
