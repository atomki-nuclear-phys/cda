# $Id$
#
# Simple script setting up the environment for running CDA
# applications.
#

# Greet the user:
echo "Setting up environment for compiling/running CDA"

# Make sure that we run in (T)CSH:
set sourced=($_)
if ( "$sourced" == "" ) then
    echo "ERROR: This script must be sourced from TCSH"
    return 1
endif

# Figure out which directory the script is in:
set thisdir=`dirname $sourced[2]`
set thisdir=`cd $thisdir; pwd`
unset sourced

# Add the bin/ and lib/ directories to the environment, if they
# exist.
if( -d $thisdir/bin ) then
    if ( ! `eval echo \$\?PATH` ) then
        setenv PATH $thisdir/bin
    else
        setenv PATH $thisdir/bin:$PATH
    endif
endif
if( -d $thisdir/lib ) then
    if ( ! `eval echo \$\?LD_LIBRARY_PATH` ) then
        setenv LD_LIBRARY_PATH $thisdir/lib
    else
        setenv LD_LIBRARY_PATH $thisdir/lib:$LD_LIBRARY_PATH
    endif
    if ( ! `eval echo \$\?DYLD_LIBRARY_PATH` ) then
        setenv DYLD_LIBRARY_PATH $thisdir/lib
    else
        setenv DYLD_LIBRARY_PATH $thisdir/lib:$DYLD_LIBRARY_PATH
    endif
endif

# Clean up:
unset thisdir
