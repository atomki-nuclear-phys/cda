
##############################################################
#                                                            #
# This project file compiles the msgserver example.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = app
TARGET   = msgserver

HEADERS =
SOURCES = msgserver.cxx

CONFIG       = qt debug warn_on
QT           = core network
INCLUDEPATH += ../../core
LIBS         = -L../../lib -lcdacore

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../bin
