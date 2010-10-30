# Dear emacs, this is normal -*- text -*-
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

TEMPLATE = subdirs
SUBDIRS  = core daq gui devices apps

#
# Instruct the user:
#
message(----------------------------------------------------------)
message(- CDA compilation configured. You should now run \"make\". -)
message(----------------------------------------------------------)
