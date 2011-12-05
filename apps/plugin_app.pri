# Dear emacs, this is normal -*- text -*-
##############################################################
#                                                            #
# This project file fragment should be used by all           #
# applications in CDA that make use of the plugins.          #
#                                                            #
# Attila Krasznahorkay Jr.                                   #
#                                                            #
##############################################################

# Link the application against all available plugins on Windows:
win32 {
   LIBS += -L../../dev -lad413a -lad1000 -lad2249a \
           -lt2228a -lt4300b -ls9418t
}
