# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles all the CDA applications.       #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common checks:
include(../cda_checks.pri)

TEMPLATE = subdirs
SUBDIRS  = cda-msgserver
