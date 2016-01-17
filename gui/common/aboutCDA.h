// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_COMMON_ABOUTCDA_H
#define CDA_GUI_COMMON_ABOUTCDA_H

// Qt include(s):
#include <QtCore/QtGlobal>

// Local include(s):
#include "Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QWidget )

/// Show an information window about CDA
CDAGUI_EXPORT void aboutCDA( QT_PREPEND_NAMESPACE( QWidget )* parent );

#endif // CDA_GUI_COMMON_ABOUTCDA_H
