# Dear emacs, this is -*- fundamental -*- text
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
CONFIG += qt
CONFIG -= app_bundle
QT      = core network xml gui widgets

# The places to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../bin

# The include paths:
INCLUDEPATH += ../../core ../../daq ../../gui

# The library dependencies:
mac {
   QMAKE_CXXFLAGS += -F$$OUT_PWD/../../lib
   LIBS           += -F$$OUT_PWD/../../lib -framework cdacore -framework cdadaq
}
!mac {
   LIBS += -L../../lib -lcdadaq -lcdacore
}
contains(DEFINES,HAVE_CERNLIB) {
   LIBS += $$system(cernlib packlib)
}
