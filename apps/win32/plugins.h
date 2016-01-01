// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APP_WIN32_PLUGINS_H
#define CDA_APP_WIN32_PLUGINS_H

// Qt include(s):
#include <QtCore/QtPlugin>

// On Windows all the plugins have to be statically linked
// to the applications that need them. The following activates
// the static linking.

#if QT_VERSION < QT_VERSION_CHECK( 5, 0, 0 )

#if defined(Q_OS_WIN) || defined(HAVE_CERNLIB)
Q_IMPORT_PLUGIN( ad413a )
Q_IMPORT_PLUGIN( ad1000 )
Q_IMPORT_PLUGIN( ad2249a )
Q_IMPORT_PLUGIN( t2228a )
Q_IMPORT_PLUGIN( t4300b )
Q_IMPORT_PLUGIN( s9418t )
Q_IMPORT_PLUGIN( dt5740 )
#endif // Q_OS_WIN || HAVE_CERNLIB

#else

#if defined(Q_OS_WIN) || defined(HAVE_CERNLIB)
Q_IMPORT_PLUGIN( Ad413aFactory )
Q_IMPORT_PLUGIN( Ad1000Factory )
Q_IMPORT_PLUGIN( Ad2249aFactory )
Q_IMPORT_PLUGIN( T2228aFactory )
Q_IMPORT_PLUGIN( T4300bFactory )
Q_IMPORT_PLUGIN( S9418tFactory )
Q_IMPORT_PLUGIN( Dt5740Factory )
#endif // Q_OS_WIN || HAVE_CERNLIB

#endif // QT_VERSION

#endif // CDA_APP_WIN32_PLUGINS_H
