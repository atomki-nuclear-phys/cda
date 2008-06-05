
##############################################################
#                                                            #
# This project file compiles the core library of CDA.        #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

TEMPLATE = lib
VERSION  = 0.0.1
TARGET   = cdacore

HEADERS = fifo/*.h msg/*.h event/*.h device/*.h
SOURCES = fifo/*.cxx msg/*.cxx event/*.cxx device/*.cxx

CONFIG = qt debug warn_on shared
QT     = core network gui

OBJECTS_DIR = ./build
MOC_DIR     = ./build
DESTDIR     = ../lib
