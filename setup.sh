# $Id$
#
# Simple script setting up the environment for running CDA
# applications/examples.
#

echo "Setting up environment for compiling/running CDA"

# Set the environment variables:
export CDASYS=$PWD
export PATH=$CDASYS/bin:$PATH
if [ ! $LD_LIBRARY_PATH ]; then
    export LD_LIBRARY_PATH=$CDASYS/lib
else
    export LD_LIBRARY_PATH=$CDASYS/lib:$LD_LIBRARY_PATH
fi

# Check that the directory for FIFOs exists (It's not in the repository...)
if [ ! -d $CDASYS/fifos ]; then
    echo Directory $CDASYS/fifos does not exist. Creating it...
    mkdir $CDASYS/fifos
fi

# Check that the FIFOs themselves exist. (They're not in the repository...)
if [ ! -p $CDASYS/fifos/histFifo ]; then
    echo FIFO $CDASYS/fifos/histFifo does not exist. Creating it...
    mkfifo $CDASYS/fifos/histFifo
    chmod 666 $CDASYS/fifos/histFifo
fi
