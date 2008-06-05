
TEMPLATE = lib
VERSION  = 0.0.1
TARGET   = example

HEADERS = *.h
SOURCES = *.cxx

CONFIG       = qt debug warn_on plugin
QT           = core gui xml
INCLUDEPATH += ../../core
LIBS         = -L../../lib -lcdacore

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../../dev
