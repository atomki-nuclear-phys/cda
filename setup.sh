
#
# Simple script setting up the environment for running CDA
# applications/examples.
#

export CDASYS=$PWD
export PATH=$CDASYS/bin:$PATH
export LD_LIBRARY_PATH=$CDASYS/lib:$LD_LIBRARY_PATH
# To support Mac OS X:
export DYLD_LIBRARY_PATH=$CDASYS/lib:$DYLD_LIBRARY_PATH
