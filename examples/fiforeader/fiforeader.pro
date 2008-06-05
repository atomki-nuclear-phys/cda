
##############################################################
#                                                            #
# This project file compiles the fiforeader example.         #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = app
TARGET   = fiforeader

HEADERS =
SOURCES = fiforeader.cxx

CONFIG       = qt debug warn_on
QT           = core network
INCLUDEPATH += ../../core
LIBS         = -L../../lib -lcdacore

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../bin
