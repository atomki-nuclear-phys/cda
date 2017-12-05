// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_COMMON_EXPORT_H
#define CDA_CORE_COMMON_EXPORT_H

// Qt include(s):
#include <QtGlobal>

/// Helper macro for declaring functions/classes to be visible on all platforms
#ifdef CDACORE_LIBRARY
#   define CDACORE_EXPORT
#else
#   define CDACORE_EXPORT
#endif

#endif // CDA_CORE_COMMON_EXPORT_H
