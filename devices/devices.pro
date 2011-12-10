# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles all the devices making use of   #
# the core CDA library.                                      #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../cda_common.pri)

TEMPLATE = subdirs

# The CAMAC device(s):
SUBDIRS  = t2228a ad2249a t4300b ad413a ad1000

# The VME device(s):
SUBDIRS += s9418t

# The CAEN device(s):
SUBDIRS += dt5740
