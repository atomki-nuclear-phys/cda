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

# Generic application(s):
SUBDIRS  = cda-msgserver cda-config-editor \
           cda-stat-server cda-raw-writer cda-config-server

# CAMAC specific application(s):
SUBDIRS += cda-camac-reader cda-camac-daq

# CAEN specific application(s):
SUBDIRS +=  cda-caen-reader cda-caen-daq

# Only compile the applications needing CERNLIB if that is in fact
# available:
contains(DEFINES,HAVE_CERNLIB) {
   SUBDIRS += cda-glomem-writer cda-hbook-writer
}

# Only compile the application(s) needing ROOT, if that is in
# fact available:
contains(DEFINES,HAVE_ROOT_LIBS) {
   SUBDIR += cda-root-writer
}
