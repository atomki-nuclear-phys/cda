# Dear emacs, this is -*- fundamental -*- text
#
# $Id$
#
# This project file is used just to "configure" the .qmake.cache file,
# so that it could be loaded by all the subdirectories during the build.
#

TEMPLATE = subdirs
SUBDIRS =

QMAKE_SUBSTITUTES += .qmake.cache.in
