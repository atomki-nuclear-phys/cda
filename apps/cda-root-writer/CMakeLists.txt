#
# Configuration for building the cda-root-writer application.
#

# Don't build this executable if ROOT is not used:
if( NOT CDA_USING_ROOT )
   return()
endif()

# Use the helper macro to set up the build:
cda_add_console_executable( cda-root-writer )
