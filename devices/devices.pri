# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file fragment should be used by all device    #
# plug-ins compiled for CDA.                                 #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# The plugin is actually a library:
TEMPLATE = lib

# The plugins use the QtCore, QtGui and QtXml libraries:
CONFIG += qt
QT      = core gui widgets xml network

# On Windows we compile static plugins, but on Linux/MacOS X,
# we compile dynamic loading plugins instead.
win32 {
   CONFIG += static plugin
}
!win32 {
   CONFIG += plugin
}

# The place to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../dev

# The include paths:
INCLUDEPATH += ../../core ../../daq

# The library dependencies:
mac {
   QMAKE_CXXFLAGS += -F../../lib
   LIBS           += -F../../lib -framework cdacore -framework cdadaq
}
!mac {
   LIBS += -L../../lib -lcdacore -lcdadaq
}
