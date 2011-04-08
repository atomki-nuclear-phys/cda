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
CONFIG += qt plugin
QT      = core gui xml

# The place to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../dev
