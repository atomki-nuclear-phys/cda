#
# Configuration for building the cda-glomem-writer application.
#

# Don't build this executable if CERNLIB is not used:
if( NOT CDA_USING_CERNLIB )
   return()
endif()

# Use the helper macro to set up the build:
cda_add_console_executable( cda-glomem-writer )
