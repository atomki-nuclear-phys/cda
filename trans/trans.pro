# Dear emacs, this is normal -*- text -*-
#
# This file is just here to make it simple to create the .qm
# translations from all the .tm files with one command.
# You should only run "lrelease" in this directory, "lupdate" should
# always be run from the main directory.

TRANSLATIONS = $$files(*.ts)
