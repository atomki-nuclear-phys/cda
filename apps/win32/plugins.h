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
Q_IMPORT_PLUGIN( v862 )
Q_IMPORT_PLUGIN( v775 )
Q_IMPORT_PLUGIN( v785 )
Q_IMPORT_PLUGIN( v792 )
Q_IMPORT_PLUGIN( v812 )

#else

Q_IMPORT_PLUGIN( Ad413aFactory )
Q_IMPORT_PLUGIN( Ad1000Factory )
Q_IMPORT_PLUGIN( Ad2249aFactory )
Q_IMPORT_PLUGIN( T2228aFactory )
Q_IMPORT_PLUGIN( T4300bFactory )
Q_IMPORT_PLUGIN( Dt5740Factory )
Q_IMPORT_PLUGIN( V862Factory )
Q_IMPORT_PLUGIN( V775Factory )
Q_IMPORT_PLUGIN( V785Factory )
Q_IMPORT_PLUGIN( V792Factory )
Q_IMPORT_PLUGIN( V812Factory )

#endif // QT_VERSION

#endif // CDA_APP_WIN32_PLUGINS_H
