// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_COMMON_EXPORT_H
#define CDA_DAQ_COMMON_EXPORT_H

// Qt include(s):
#include <QtCore/QtGlobal>

/// Helper macro for declaring functions/classes to be visible on all platforms
#ifdef CDADAQ_LIBRARY
#   define CDADAQ_EXPORT
#else
#   define CDADAQ_EXPORT
#endif

#endif // CDA_DAQ_COMMON_EXPORT_H
