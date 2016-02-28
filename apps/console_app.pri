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
CONFIG += console
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
   QMAKE_CXXFLAGS += -F$$CDASYS/lib
   LIBS           += -F$$CDASYS/lib -framework cdacore -framework cdadaq
}
unix:!mac {
   LIBS += -L../../lib -lcdadaq -lcdacore
}
win32 {
   LIBS += -L../../bin -lcdadaq1 -lcdacore1
}
contains(DEFINES,HAVE_CERNLIB) {
   LIBS += $$system(cernlib packlib)
   contains(DEFINES,HAVE_ROOT_LIBS) {
      LIBS += $$system(root-config --libs) -lMathCore
   }
   contains(DEFINES,HAVE_CAMAC_LIB) {
      LIBS += -lcc32
   }
   contains(DEFINES,HAVE_CAEN_DIGITIZER_LIBS) {
      LIBS += -lCAENVME -lCAENComm -lCAENDigitizer
   }
   contains(DEFINES,HAVE_VME_LIB) {
      LIBS += -lpcivme
   }
}

# Make the built executables work without an environment setting in the
# build dir:
QMAKE_RPATHDIR += $$CDASYS/lib
QMAKE_RPATHLINKDIR += $$CDASYS/lib
