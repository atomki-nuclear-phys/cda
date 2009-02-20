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
VERSION  = 0.0.2
TARGET   = cdacore

# These are the header and source files:
HEADERS = fifo/*.h msg/*.h event/*.h device/*.h camac/*.h cmdl/*.h \
          cernlib/*.h common/*.h
SOURCES = fifo/*.cxx msg/*.cxx event/*.cxx device/*.cxx camac/*.cxx \
          cmdl/*.cpp cernlib/*.cxx common/*.cxx
TRANSLATIONS = cdacore_hu.ts

# The library uses the QtCore, QtNetwork and QtGui libraries:
CONFIG += qt
QT      = core network gui
LIBS   += -L$$CERNLIB_PATH/lib

# The place to put the intermediate build results:
OBJECTS_DIR = ./.obj
MOC_DIR     = ./.obj

# The following is needed to compiled the cmdl code:
DEFINES += unix_style

#
# These are the specific configuration options for compiling the code
# on Mac OS X.
#
mac {
   message(*)
   message(* Configuring to build the \"cdacore\" framework on Mac OS X)
   message(*)

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
   DEVICE_HEADERS.files = $$system(ls device/*.h device/*.icc)
   DEVICE_HEADERS.path = Headers/device
   QMAKE_BUNDLE_DATA += DEVICE_HEADERS

   EVENT_HEADERS.version = Versions
   EVENT_HEADERS.files = $$system(ls event/*.h)
   EVENT_HEADERS.path = Headers/event
   QMAKE_BUNDLE_DATA += EVENT_HEADERS

   FIFO_HEADERS.version = Versions
   FIFO_HEADERS.files = $$system(ls fifo/*.h)
   FIFO_HEADERS.path = Headers/fifo
   QMAKE_BUNDLE_DATA += FIFO_HEADERS

   MSG_HEADERS.version = Versions
   MSG_HEADERS.files = $$system(ls msg/*.h)
   MSG_HEADERS.path = Headers/msg
   QMAKE_BUNDLE_DATA += MSG_HEADERS

   CAMAC_HEADERS.version = Versions
   CAMAC_HEADERS.files = $$system(ls camac/*.h)
   CAMAC_HEADERS.path = Headers/camac
   QMAKE_BUNDLE_DATA += CAMAC_HEADERS

   CMDL_HEADERS.version = Versions
   CMDL_HEADERS.files = cmdl/cmdargs.h cmdl/cmdline.h
   CMDL_HEADERS.path = Headers/cmdl
   QMAKE_BUNDLE_DATA += CMDL_HEADERS

   CERNLIB_HEADERS.version = Versions
   CERNLIB_HEADERS.files = $$system(ls cernlib/*.h)
   CERNLIB_HEADERS.path = Headers/cernlib
   QMAKE_BUNDLE_DATA += CERNLIB_HEADERS

   COMMON_HEADERS.version = Versions
   COMMON_HEADERS.files = $$system(ls common/*.h)
   COMMON_HEADERS.path = Headers/common
   QMAKE_BUNDLE_DATA += COMMON_HEADERS

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
   message(*)
   message(* Configuring to build the \"cdacore\" shared library on Linux)
   message(*)

   CONFIG += shared
   DESTDIR = ../lib

}
system(g77 --version){
	LIBS += -L/usr/lib/gcc/i386-redhat-linux/3.4.6 -lg2c -lgfortran
} else{
	!system(gfortran --version){
		warning(Could not determine which fortran library to use)
	}
	LIBS += -lgfortran
}

exists($$CERNLIB_PATH/lib/libpacklib_noshift.a){
	LIBS += -lpacklib_noshift
} else{
	!exists($$CERNLIB_PATH/lib/libpacklib.a){
		warning(Could not determine the packlib library name)
	}
	
	LIBS += -lpacklib
}
