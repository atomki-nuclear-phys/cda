# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file fragment should be used by all           #
# applications in CDA that make use of the plugins.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Link the application against all available plugins on Windows,
# or when using cernlib. (And hence building the plugins as static
# libraries.)
contains(DEFINES,HAVE_CERNLIB) {
   # Statically link applications against the plugins:
   LIBS += -L../../dev -lad413a -lad1000 -lad2249a \
           -lt2228a -lt4300b -ls9418t -ldt5740

   # Define the plugin resources explicitly for the
   # applications, because they are not loaded properly from
   # the static plugins for some reason...
   RESOURCES   += ../../devices/dt5740/dt5740.qrc
}
