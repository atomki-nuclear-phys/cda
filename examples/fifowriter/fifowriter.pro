
##############################################################
#                                                            #
# This project file compiles the fifowriter example.         #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = app
TARGET   = fifowriter

HEADERS =
SOURCES = fifowriter.cxx

CONFIG       = qt debug warn_on
QT           = core network
INCLUDEPATH += ../../core
LIBS         = -L../../lib -lcdacore

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../bin
