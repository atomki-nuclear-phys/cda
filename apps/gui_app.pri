# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This project file fragment should be used by all GUI       #
# applications in CDA.                                       #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# This will be an application:
TEMPLATE = app

# The application uses a bunch of Qt libraries:
CONFIG += qt
QT      = core network xml gui widgets

# The places to put the intermediate and final build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj
DESTDIR     = ../../bin

# Special linking setup for Windows:
QMAKE_LFLAGS_WINDOWS += -Wl,--enable-auto-import

# The include paths:
INCLUDEPATH += ../../core ../../daq ../../gui

# The library dependencies:
mac {
   QMAKE_CXXFLAGS += -F$$OUT_PWD/../../lib
   LIBS           += -F$$OUT_PWD/../../lib -framework cdacore -framework cdadaq -framework cdagui
}
!mac {
   LIBS += -L../../lib -lcdagui -lcdadaq -lcdacore
}
contains(DEFINES,HAVE_CERNLIB) {
   LIBS += $$system(cernlib packlib)
}
