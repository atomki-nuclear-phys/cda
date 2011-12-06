// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_COMMON_SLEEP_H
#define CDA_CORE_COMMON_SLEEP_H

namespace common {

   /// Platform independent function for sleeping for a while
   void Sleep( unsigned int milisec );

   /// Sleep function sleeping for the shortest possible time
   void SleepMin();

} // namespace common

#endif // CDA_CORE_COMMON_SLEEP_H
