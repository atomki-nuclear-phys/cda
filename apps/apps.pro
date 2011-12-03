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

# These are the applications that only depend on Qt:
SUBDIRS  = cda-msgserver cda-camac-reader cda-config-editor \
           cda-stat-server cda-raw-writer cda-config-server \
           cda-caen-reader

# Only compile the applications needing CERNLIB if that is in fact
# available:
contains(DEFINES,HAVE_CERNLIB) {
   SUBDIRS += cda-glomem-writer cda-hbook-writer cda-simple-daq
}
