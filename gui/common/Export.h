// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_COMMON_EXPORT_H
#define CDA_GUI_COMMON_EXPORT_H

// Qt include(s):
#include <QtGlobal>

/// Helper macro for declaring functions/classes to be visible on all platforms
#ifdef CDAGUI_LIBRARY
#define CDAGUI_EXPORT
#else
#define CDAGUI_EXPORT
#endif

#endif  // CDA_GUI_COMMON_EXPORT_H
