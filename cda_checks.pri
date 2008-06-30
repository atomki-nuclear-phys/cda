# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# Some common checks that should be included in all the      #
# project files.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Check that we are either on Linux or on a Mac:
!linux:!mac {
   error(CDA is only supported on Linux and Mac OS X!)
}

# Check the version of Qt:
!contains(QT_MAJOR_VERSION,4) {
   warning(Your Qt version is $$QT_VERSION)
   error(CDA has to be built with Qt version 4.4!)
}
!contains(QT_MINOR_VERSION,4) {
   warning(Your Qt version is $$QT_VERSION)
   error(CDA has to be built with Qt version 4.4!)
}

# Check that the CDASYS environment variable exists:
cdasys = $$system(echo $CDASYS)
isEmpty(cdasys) {
   error(CDASYS not defined. Please source one of the setup scripts!)
}
