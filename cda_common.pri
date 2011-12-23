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
# Qt 4.X.
#
!contains(QT_MAJOR_VERSION,4) {
   isEmpty($$QT_VERSION) {
      message(Your Qt version is too old. Probably 3.X...)
   } else {
      message(Your Qt version is $$QT_VERSION)
   }
   error(CDA has to be built with Qt version 4.X!)
}

#
# CDA is only supported on Linux and Mac OS X (the two systems
# I use for development). This code snippet doesn't allow to
# compile it on Windows:
#
#QMAKE_CXXFLAGS_DEBUG="-ggdb -O0 -fno-inline -W -Wall"
#!unix:!mac {
#   error(CDA is only supported on Linux and Mac OS X!)
#}

#
# During development the code is compiled in debug mode to make it easier
# to find problems in it. But the programs used for real data acquisition
# should not carry around the debug information... (Add "debug" in the following
# list if you want to debug the code!)
#
CONFIG += warn_on release

#
# Check that the CDASYS environment variable exists. The value of
# the CDASYS environment variable is compiled into the applications
# as a fallback solution if it is not set when running them. Also,
# this variable is heavily used when compiling the code on
# Mac OS X. All in all, compilation can't commence without it.
#
unix {
   CDASYS = $$system(echo $CDASYS)
   isEmpty(CDASYS) {
      error(CDASYS not defined. Please source one of the setup scripts!)
   }
   DEFINES += CDASYS=$$CDASYS
}
win32 {
   DEFINES += CDASYS=$$PWD
}

#
# Uncomment the following if you have the CAMAC library on your
# system:
#
#DEFINES += HAVE_CAMAC_LIB

#
# Uncomment the following if you have the CAEN libraries installed
# on your system.
#
DEFINES += HAVE_CAEN_LIBS

#
# Uncomment the following if you have the VME library installed
# on your system.
#
#DEFINES += HAVE_VME_LIB

#
# Uncomment the following if you have CERNLIB installed on your
# system.
#
DEFINES += HAVE_CERNLIB

#
# Uncomment the following if you have ROOT installed on your
# system.
#
#DEFINES += HAVE_ROOT_LIBS

#
# Compiler settings. In order to be able to link against CERNLIB on
# Mac OS X, the built-in version of GCC can't be used. In my case
# I installed CERNLIB using the Fink project. (www.finkproject.org)
# That package manager compiles the CERNLIB libraries with GCC 4.3,
# so I make the appropriate settings for that here. The setting for
# linux is only here for completeness, normally it shouldn't be
# tweaked.
#
#QMAKE_CC         = gcc-4
#QMAKE_CXX        = g++-4
#QMAKE_LINK       = g++-4
#QMAKE_LINK_SHLIB = g++-4
