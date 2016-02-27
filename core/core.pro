# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles the core library of CDA.        #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../cda_common.pri)

# This will be a library with the name "cdacore":
TEMPLATE = lib
VERSION  = 1.1.0
TARGET   = cdacore

# These are the header and source files:
HEADERS = $$files(fifo/*.h) $$files(msg/*.h) $$files(event/*.h) \
          $$files(device/*.h) $$files(device/*.icc) $$files(camac/*.h) \
          $$files(cmdl/*.h) $$files(cernlib/*.h) $$files(common/*.h) \
          $$files(i18n/*.h) $$files(vme/*.h) $$files(caen/*.h) \
          $$files(root/*.h) $$files(root/*.icc) $$files(tclap/*.h)
SOURCES = $$files(fifo/*.cxx) $$files(msg/*.cxx) $$files(event/*.cxx) \
          $$files(device/*.cxx) $$files(camac/*.cxx) $$files(cmdl/*.cpp) \
          $$files(cernlib/*.cxx) $$files(common/*.cxx) $$files(i18n/*.cxx) \
          $$files(vme/*.cxx) $$files(caen/*.cxx) $$files(root/*.cxx)
TRANSLATIONS = ../trans/cdacore_hu.ts

# The library uses the QtCore, QtNetwork and QtGui libraries:
CONFIG += qt
QT      = core network gui widgets

# The place to put the intermediate build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj

# The following is needed to compile the cmdl code:
DEFINES += unix_style

# In order to export the library's symbols:
DEFINES += CDACORE_LIBRARY

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   # We create a library bundle and create a file storing the dependencies
   # of this bundle:
   CONFIG += lib_bundle create_prl

   # The headers are added to the framework in a fancy way. In order to
   # retain the original directory structure, the headers from each
   # sub-directory are added separately.
   #
   # Additionally, a dummy header description is added to make the
   # framework's layout correct.

   DEVICE_HEADERS.version = Versions
   DEVICE_HEADERS.files = $$files(device/*.h) $$files(device/*.icc)
   DEVICE_HEADERS.path = Headers/device
   QMAKE_BUNDLE_DATA += DEVICE_HEADERS

   EVENT_HEADERS.version = Versions
   EVENT_HEADERS.files = $$files(event/*.h)
   EVENT_HEADERS.path = Headers/event
   QMAKE_BUNDLE_DATA += EVENT_HEADERS

   FIFO_HEADERS.version = Versions
   FIFO_HEADERS.files = $$files(fifo/*.h)
   FIFO_HEADERS.path = Headers/fifo
   QMAKE_BUNDLE_DATA += FIFO_HEADERS

   MSG_HEADERS.version = Versions
   MSG_HEADERS.files = $$files(msg/*.h)
   MSG_HEADERS.path = Headers/msg
   QMAKE_BUNDLE_DATA += MSG_HEADERS

   CAMAC_HEADERS.version = Versions
   CAMAC_HEADERS.files = $$files(camac/*.h)
   CAMAC_HEADERS.path = Headers/camac
   QMAKE_BUNDLE_DATA += CAMAC_HEADERS

   VME_HEADERS.version = Versions
   VME_HEADERS.files = $$files(vme/*.h)
   VME_HEADERS.path = Headers/vme
   QMAKE_BUNDLE_DATA += VME_HEADERS

   CAEN_HEADERS.version = Versions
   CAEN_HEADERS.files = $$files(caen/*.h) $$files(caen/*.icc)
   CAEN_HEADERS.path = Headers/caen
   QMAKE_BUNDLE_DATA += CAEN_HEADERS

   CMDL_HEADERS.version = Versions
   CMDL_HEADERS.files = cmdl/cmdargs.h cmdl/cmdline.h
   CMDL_HEADERS.path = Headers/cmdl
   QMAKE_BUNDLE_DATA += CMDL_HEADERS

   TCLAP_HEADERS.version = Versions
   TCLAP_HEADERS.files = $$files(tclap/*.h)
   TCLAP_HEADERS.path = Headers/tclap
   QMAKE_BUNDLE_DATA += TCLAP_HEADERS

   CERNLIB_HEADERS.version = Versions
   CERNLIB_HEADERS.files = $$files(cernlib/*.h)
   CERNLIB_HEADERS.path = Headers/cernlib
   QMAKE_BUNDLE_DATA += CERNLIB_HEADERS

   COMMON_HEADERS.version = Versions
   COMMON_HEADERS.files = $$files(common/*.h)
   COMMON_HEADERS.path = Headers/common
   QMAKE_BUNDLE_DATA += COMMON_HEADERS

   I18N_HEADERS.version = Versions
   I18N_HEADERS.files = $$files(i18n/*.h)
   I18N_HEADERS.path = Headers/i18n
   QMAKE_BUNDLE_DATA += I18N_HEADERS

   ROOT_HEADERS.version = Versions
   ROOT_HEADERS.files = $$files(root/*.h) $$files(root/*.icc)
   ROOT_HEADERS.path = Headers/root
   QMAKE_BUNDLE_DATA += ROOT_HEADERS

   DUMMY_HEADERS.version = Versions
   DUMMY_HEADERS.files =
   DUMMY_HEADERS.path = Headers
   QMAKE_BUNDLE_DATA += DUMMY_HEADERS

   # Again, these are some fancy configuration options. On Mac OS X there
   # is no environment preference for finding frameworks. The frameworks
   # actually all store an "installed name" preference that tells the other
   # frameworks or applications linking against them, where to find them
   # later on. By default this installed name would be something like
   # "cdacore.framework/Versions/0/cdacore" from which the applications
   # linked against this framework would have no idea where to find it...
   # So instead I set the framework bundle name to contain the full path
   # name of the framework. This way all the applications linked against it
   # will be looking for it in its correct directory. (Of course moving the
   # framework after compilation will make the applications unusable.)
   DESTDIR                     = /
   QMAKE_FRAMEWORK_BUNDLE_NAME = $$CDASYS/lib/cdacore

}

#
# These are the configuration options for compiling the code on Linux.
# Way simpler than the Mac OS X ones...
#
unix:!mac {
   CONFIG += shared
   DESTDIR = ../lib
}

#
# These are the configuration options for compiling the code on Windows:
#
win32 {
   CONFIG += shared
   DESTDIR = ../bin
}

#
# Decide whether to link the library again CERNLIB, and if yes, how:
#
contains(DEFINES,HAVE_CERNLIB) {
   mac {
      CERNLIB_PATH = /sw
   }
   unix:!mac {
      CONFIG += staticlib
      exists(/cern/pro){
         CERNLIB_PATH = /cern/pro
      } else {
         CERNLIB_PATH = /usr
      }
   }
   DEFINES     += LINUX f2cFortran
   INCLUDEPATH += $$CERNLIB_PATH/include $$CERNLIB_PATH/include/cfortran
}

#
# Decide whether to link the library against the ROOT libraries:
#
contains(DEFINES,HAVE_ROOT_LIBS) {
   QMAKE_CXXFLAGS += $$system(root-config --cflags)
   LIBS += $$system(root-config --libs) -lMathCore
}

#
# Decide whether to link the library against the CAMAC library:
#
contains(DEFINES,HAVE_CAMAC_LIB) {
   LIBS += -lcc32
}

#
# Decide whether to link the library against the CAEN digitiser libraries:
#
contains(DEFINES,HAVE_CAEN_DIGITIZER_LIBS) {
   win32 {
      INCLUDEPATH += "/Program Files/CAEN/Digitizers/Library/include" \
                     "/Program Files/CAEN/Comm/include" \
                     "/Program Files/CAEN/VME/include"
      LIBS += -L"/Program Files/CAEN/Digitizers/Library/lib/x86_64" \
              -L"/Program Files/CAEN/Comm/lib/x86_64" \
              -L"/Program Files/CAEN/VME/lib/x86_64" \
              -lCAENDigitizer -lCAENComm -lCAENVMElib
   } else {
      LIBS += -lCAENVME -lCAENComm -lCAENDigitizer
   }
}

#
# Decide whether to link the library against the CAEN QTP libraries:
#
contains(DEFINES,HAVE_CAEN_QTP_LIBS) {
   LIBS += -lCAENVME -lCAENqtpSDK
}

#
# Decide whether to link the library against the VME library:
#
contains(DEFINES,HAVE_VME_LIB) {
   LIBS += -lpcivme
}
