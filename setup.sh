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

# Check that all the directories for the compiled binaries exist.
# (They're no longer in the repository...)
if [ ! -d $CDASYS/bin ]; then
    echo Directory $CDASYS/bin does not exist. Creating it...
    mkdir $CDASYS/bin
fi
if [ ! -d $CDASYS/lib ]; then
    echo Directory $CDASYS/lib does not exist. Creating it...
    mkdir $CDASYS/lib
fi
if [ ! -d $CDASYS/dev ]; then
    echo Directory $CDASYS/dev does not exist. Creating it...
    mkdir $CDASYS/dev
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
