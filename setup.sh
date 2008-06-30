# $Id$
#
# Simple script setting up the environment for running CDA
# applications/examples.
#

echo -n "Setting up environment for compiling/running CDA ... "

export CDASYS=$PWD
export PATH=$CDASYS/bin:$PATH
if [ ! $LD_LIBRARY_PATH ]; then
    export LD_LIBRARY_PATH=$CDASYS/lib
else
    export LD_LIBRARY_PATH=$CDASYS/lib:$LD_LIBRARY_PATH
fi

echo "done"
