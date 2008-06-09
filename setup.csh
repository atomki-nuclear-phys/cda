
#
# Simple script setting up the environment for running CDA
# applications/examples.
#

setenv CDASYS $PWD
setenv PATH $CDASYS/bin:$PATH
setenv LD_LIBRARY_PATH $CDASYS/lib:$LD_LIBRARY_PATH
# To support Mac OS X:
setenv DYLD_LIBRARY_PATH $CDASYS/lib:$DYLD_LIBRARY_PATH
