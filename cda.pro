# Dear emacs, this is -*- fundamental -*- text
##############################################################
#                                                            #
# This is the main project directory for compiling the whole #
# CDA application suit.                                      #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(cda_common.pri)

# Define how (where) to compile the sources:
TEMPLATE = subdirs
SUBDIRS  = core daq gui devices apps

# Define where to install the full software:
CDA_INSTALL_PREFIX = /usr/local/cda
CDA_BIN.path  = $$CDA_INSTALL_PREFIX/bin
CDA_BIN.files = $$files(bin/*)
CDA_LIB.path  = $$CDA_INSTALL_PREFIX/lib
CDA_LIB.files = $$files(lib/*)
CDA_DEV.path  = $$CDA_INSTALL_PREFIX/dev
CDA_DEV.files = $$files(dev/*)
CDA_SET.path  = $$CDA_INSTALL_PREFIX
CDA_SET.files = setup.sh setup.csh
INSTALLS += CDA_BIN CDA_LIB CDA_DEV CDA_SET

#
# Instruct the user:
#
message(----------------------------------------------------------)
message(- CDA compilation configured. You should now run \"make\". -)
message(----------------------------------------------------------)
