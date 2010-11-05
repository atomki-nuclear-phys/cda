// $Id$

// Local include(s):
#include "Event.h"

namespace ev {

   Event::Event()
      : m_fragments() {

   }

   const std::vector< Fragment >& Event::getFragments() const {

      return m_fragments;
   }

   void Event::addFragment( const Fragment& fragment ) {

      m_fragments.push_back( fragment );
      return;
   }

   void Event::clear() {

      m_fragments.clear();
      return;
   }

} // namespace ev
