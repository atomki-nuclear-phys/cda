# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file fragment should be used by all "console" #
# applications in CDA.                                       #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# This will be an application:
TEMPLATE = app

# The application uses a bunch of Qt libraries:
CONFIG = qt
QT     = core network xml gui

# The places to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../bin
