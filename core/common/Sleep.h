// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_SLEEP_H
#define CDA_CORE_COMMON_SLEEP_H

// CDA include(s):
#include "Export.h"

namespace common {

   /// Platform independent function for sleeping for a while
   CDACORE_EXPORT void Sleep( unsigned int milisec );

   /// Sleep function sleeping for the shortest possible time
   CDACORE_EXPORT void SleepMin();

} // namespace common

#endif // CDA_CORE_COMMON_SLEEP_H
