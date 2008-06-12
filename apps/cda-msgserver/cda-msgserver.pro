
##############################################################
#                                                            #
# This project file compiles the msgserver example.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = app
TARGET   = cda-msgserver

HEADERS = *.h
SOURCES = *.cxx

CONFIG      += qt debug warn_on
QT           = core gui network
INCLUDEPATH += ../../core ../../gui
LIBS         = -L../../lib -lcdacore -lcdagui
RESOURCES    = cda-msgserver.qrc

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../bin
