# Dear emacs, this is normal -*- text -*-
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
QT      = core gui xml

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
