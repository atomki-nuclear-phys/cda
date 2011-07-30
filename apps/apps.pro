# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file compiles all the CDA applications.       #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Include the common project options:
include(../cda_common.pri)

TEMPLATE = subdirs
SUBDIRS  = cda-msgserver cda-camac-reader cda-glomem-writer \
           cda-config-editor cda-hbook-writer cda-simple-daq \
           cda-stat-server cda-raw-writer
