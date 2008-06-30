# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This is the main project directory for compiling the whole #
# CDA application suit.                                      #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common checks:
include(cda_checks.pri)

TEMPLATE = subdirs
SUBDIRS  = core gui devices apps examples
