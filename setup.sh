# $Id$
#
# Simple script setting up the environment for running CDA
# applications.
#

# Greet the user:
echo "Setting up environment for compiling/running CDA"

# Figure out which directory the script is in:
if [ "x${BASH_SOURCE[0]}" = "x" ]; then
    # This trick should do the right thing under ZSH:
    thisdir=$(dirname `print -P %x`)
    if [ $? != 0 ]; then
        echo "ERROR: This script must be sourced from BASH or ZSH"
        return 1
    fi
else
    # The BASH solution is a bit more straight forward:
    thisdir=$(dirname ${BASH_SOURCE[0]})
fi
thisdir=$(cd $thisdir; pwd)

# Add the bin/ and lib/ directories to the environment, if they
# exist.
if [ -d ${thisdir}/bin ]; then
    if [ -z "${PATH}" ]; then
        export PATH=${thisdir}/bin
    else
        export PATH=${thisdir}/bin:${PATH}
    fi
fi
if [ -d ${thisdir}/lib ]; then
    if [ -z "${LD_LIBRARY_PATH}" ]; then
        export LD_LIBRARY_PATH=${thisdir}/lib
    else
        export LD_LIBRARY_PATH=${thisdir}/lib:${LD_LIBRARY_PATH}
    fi
    if [ -z "${DYLD_LIBRARY_PATH}" ]; then
        export DYLD_LIBRARY_PATH=${thisdir}/lib
    else
        export DYLD_LIBRARY_PATH=${thisdir}/lib:${DYLD_LIBRARY_PATH}
    fi
fi

# Clean up:
unset thisdir
