# $Id$
#
# Simple script setting up the environment for running CDA
# applications/examples.
#

echo "Setting up environment for compiling/running CDA"

# Set the environment variables:
setenv CDASYS $PWD
setenv PATH $CDASYS/bin:$PATH
if( $?LD_LIBRARY_PATH ) then
    setenv LD_LIBRARY_PATH $CDASYS/lib:$LD_LIBRARY_PATH
else
    setenv LD_LIBRARY_PATH $CDASYS/lib
endif

# Check that all the directories for the compiled binaries exist.
# (They're no longer in the repository...)
if( ! -d $CDASYS/bin ) then
    echo Directory $CDASYS/bin does not exist. Creating it...
    mkdir $CDASYS/bin
endif
if( ! -d $CDASYS/lib ) then
    echo Directory $CDASYS/lib does not exist. Creating it...
    mkdir $CDASYS/lib
endif
if( ! -d $CDASYS/dev ) then
    echo Directory $CDASYS/dev does not exist. Creating it...
    mkdir $CDASYS/dev
endif
