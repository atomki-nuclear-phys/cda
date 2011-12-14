// $Id$

// Local include(s):
#include "Event.h"

namespace ev {

   Event::Event()
      : m_fragments() {

   }

   Event::Event( const Event& parent )
      : m_fragments( parent.m_fragments ) {

   }

   Event& Event::operator= ( const Event& rh ) {

      m_fragments = rh.m_fragments;

      return *this;
   }

   const std::vector< std::tr1::shared_ptr< Fragment > >&
   Event::getFragments() const {

      return m_fragments;
   }

   void Event::addFragment( Fragment* fragment ) {

      m_fragments.push_back( std::tr1::shared_ptr< Fragment >( fragment ) );
      return;
   }

   void Event::clear() {

      m_fragments.clear();
      return;
   }

} // namespace ev
