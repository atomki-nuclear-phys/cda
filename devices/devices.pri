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
# we compile dynamic loading plugins instead. Unless cernlib is
# being used...
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
   QMAKE_CXXFLAGS += -F$$CDASYS/lib
   LIBS           += -F$$CDASYS/lib -framework cdacore -framework cdadaq
}
unix:!mac {
   LIBS += -L../../lib -lcdacore -lcdadaq
}
win32 {
   LIBS += -L../../bin -lcdacore1 -lcdadaq1
}

# If cernlib is being used, the plugins must be built as static libraries:
contains(DEFINES,HAVE_CERNLIB) {
   CONFIG += static
}
