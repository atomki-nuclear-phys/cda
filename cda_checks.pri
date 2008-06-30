# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# Some common checks that should be included in all the      #
# project files.                                             #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Check the version of Qt:
!contains(QT_MAJOR_VERSION,4) {
   isEmpty($$QT_VERSION) {
      message(Your Qt version is too old. Probably 3.X...)
   } else {
      message(Your Qt version is $$QT_VERSION)
   }
   error(CDA has to be built with Qt version 4.4!)
}
!contains(QT_MINOR_VERSION,4) {
   message(Your Qt version is $$QT_VERSION)
   error(CDA has to be built with Qt version 4.4!)
}

# Check that we are either on Linux or on a Mac:
!unix:!mac {
   error(CDA is only supported on Linux and Mac OS X!)
}

# Check that the CDASYS environment variable exists:
cdasys = $$system(echo $CDASYS)
isEmpty(cdasys) {
   error(CDASYS not defined. Please source one of the setup scripts!)
}
