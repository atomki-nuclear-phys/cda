
##############################################################
#                                                            #
# This project file compiles the core library of CDA.        #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = lib
VERSION  = 0.0.1
TARGET   = cdagui

HEADERS = msg/*.h
SOURCES = msg/*.cxx

CONFIG += qt debug warn_on shared
QT      = core network gui

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../lib

INCLUDEPATH += ../core
LIBS        += -L../lib -lcdacore
