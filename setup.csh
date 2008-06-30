# $Id$
#
# Simple script setting up the environment for running CDA
# applications/examples.
#

echo -n "Setting up environment for compiling/running CDA ... "

setenv CDASYS $PWD
setenv PATH $CDASYS/bin:$PATH
if( $?LD_LIBRARY_PATH ) then
    setenv LD_LIBRARY_PATH $CDASYS/lib:$LD_LIBRARY_PATH
else
    setenv LD_LIBRARY_PATH $CDASYS/lib
endif

echo "done"
