# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles all the examples making use of  #
# the core CDA libraries.                                    #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common checks:
include(../cda_checks.pri)

TEMPLATE = subdirs
SUBDIRS  = msgserver msgclient fifowriter fiforeader pluginloader
