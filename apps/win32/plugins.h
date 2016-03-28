// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APP_WIN32_PLUGINS_H
#define CDA_APP_WIN32_PLUGINS_H

// Qt include(s):
#include <QtCore/QtPlugin>

// In case the plugins need to be statically linked (e.g. in case CERNLIB is
// used), pull them in here.

#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )

Q_IMPORT_PLUGIN( ad413a )
Q_IMPORT_PLUGIN( ad1000 )
Q_IMPORT_PLUGIN( ad2249a )
Q_IMPORT_PLUGIN( t2228a )
Q_IMPORT_PLUGIN( t4300b )
Q_IMPORT_PLUGIN( dt5740 )

#else

Q_IMPORT_PLUGIN( Ad413aFactory )
Q_IMPORT_PLUGIN( Ad1000Factory )
Q_IMPORT_PLUGIN( Ad2249aFactory )
Q_IMPORT_PLUGIN( T2228aFactory )
Q_IMPORT_PLUGIN( T4300bFactory )
Q_IMPORT_PLUGIN( Dt5740Factory )

#endif // QT_VERSION

#endif // CDA_APP_WIN32_PLUGINS_H
