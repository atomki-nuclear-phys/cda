# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file fragment has to be included by all the   #
# project files in CDA. It defines common (platform-specific)#
# settings, executes a few checks of your system and so on.  #
#                                                            #
# In principle you should only have to tweak this one file   #
# when adapting the compilation settings for your computer.  #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

#
# Check the version of Qt. The code currently only supports
# Qt 4.4.
#
!contains(QT_MAJOR_VERSION,4) {
   isEmpty($$QT_VERSION) {
      message(Your Qt version is too old. Probably 3.X...)
   } else {
      message(Your Qt version is $$QT_VERSION)
   }
   error(CDA has to be built with Qt version 4.4!)
}
!contains(QT_MINOR_VERSION,4) {
   message(Your Qt version is $$QT_VERSION)
   error(CDA has to be built with Qt version 4.4!)
}

#
# CDA is only supported on Linux and Mac OS X (the two systems
# I use for development). This code snippet doesn't allow to
# compile it on Windows:
#
!unix:!mac {
   error(CDA is only supported on Linux and Mac OS X!)
}

#
# Check that the CDASYS environment variable exists. The value of
# the CDASYS environment variable is compiled into the applications
# as a fallback solution if it is not set when running them. Also,
# this variable is heavily used when compiling the code on
# Mac OS X. All in all, compilation can't commence without it.
#
CDASYS = $$system(echo $CDASYS)
isEmpty(CDASYS) {
   error(CDASYS not defined. Please source one of the setup scripts!)
}
DEFINES += CDASYS=\'\"$$CDASYS\"\'

#
# Set the compilation to "TESTING" mode. In this mode the CAMAC
# crate access is disabled and the code simulates a functioning
# CAMAC crate. Useful for developing the code "off-line".
#
# Comment-out this setting when compiling the code on the data
# acquisition computer.
#
DEFINES += TESTING

#
# If we're not in TESTING mode, everything has to be linked against
# the CAMAC library (libcc32.so):
#
!contains(DEFINES,TESTING) {
   LIBS += -lcc32
}

#
# Compiler settings. In order to be able to link against CERNLIB on
# Mac OS X, the built-in version of GCC can't be used. In my case
# I installed CERNLIB using the Fink project. (www.finkproject.org)
# That package manager compiles the CERNLIB libraries with GCC 4.3,
# so I make the appropriate settings for that here. The setting for
# linux is only here for completeness, normally it shouldn't be
# tweaked.
#
mac {
   QMAKE_CC         = gcc-4
   QMAKE_CXX        = g++-4
   QMAKE_LINK       = g++-4
   QMAKE_LINK_SHLIB = g++-4
}
unix:!mac {
   QMAKE_CC         = gcc
   QMAKE_CXX        = g++
   QMAKE_LINK       = g++
   QMAKE_LINK_SHLIB = g++
}

#
# Path to CERNLIB. CERNLIB can be installed practically anywhere,
# these settings set the variable according to the platform.
#
mac {
   CERNLIB_PATH = /sw
}
unix:!mac {
   CERNLIB_PATH = /cern/pro
}