# Dear emacs, this is -*- fundamental -*- text
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
# During development the code is compiled in debug mode to make it easier
# to find problems in it. But the programs used for real data acquisition
# should not carry around the debug information... (Add "debug" in the following
# list if you want to debug the code!)
#
CONFIG += warn_on release ordered exceptions rtti silent c++11

#
# With Qt 4.X the c++11 configuration option doesn't work yet. One has to
# use a more explicit setting.
#
contains(QT_MAJOR_VERSION,4) {
   QMAKE_CXXFLAGS += -std=c++0x
}

#
# The CDASYS variable value is compiled into the applications as a fallback
# solution in case one wants to run the code from the build area. But can be
# overridden with the CDASYS environment variable after moving/installing the
# code. But this way it's no longer necessary to source the setup script in
# order to build the code.
#
DEFINES += CDASYS=$$CDASYS

#
# Uncomment the following if you have the CAMAC library on your
# system:
#
#DEFINES += HAVE_CAMAC_LIB

#
# Uncomment the following if you have the CAEN Digitizer library installed
# on your system.
#
DEFINES += HAVE_CAEN_DIGITIZER_LIBS

#
# Uncomment the following if you have the CAEN qtp library installed
# on your system.
#
#DEFINES += HAVE_CAEN_QTP_LIBS

#
# Uncomment the following if you have the VME library installed
# on your system.
#
#DEFINES += HAVE_VME_LIB

#
# Uncomment the following if you have CERNLIB installed on your
# system.
#
#DEFINES += HAVE_CERNLIB

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
