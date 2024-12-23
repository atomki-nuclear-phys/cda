
// STL include(s):
#include <algorithm>
#include <functional>

// Local include(s):
#include "Event.h"

/// Private namespace for @c core/event/Event.cxx
namespace {

/**
 *  @short Functor calculating the size of the event
 *
 *         This is just a very fancy way of calculating the estimated
 *         size of the event in bytes, making use of the STL
 *         algorithms.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class CountBytes {

public:
   /// Constructor initializing the event size variable
   CountBytes() : m_size(0) {}

   /// Add the size of one event fragment
   void operator()(const ev::Event::Fragments_t::value_type& obj) {
      m_size += obj->sizeInBytes();
      return;
   }

   /// Get the calculated size of the event in bytes
   uint32_t size() const { return m_size; }

private:
   uint32_t m_size;  ///< Collective size of the event

};  // class CountBytes

}  // namespace

namespace ev {

const Event::Fragments_t& Event::getFragments() const {

   return m_fragments;
}

void Event::addFragment(std::unique_ptr<Fragment> fragment) {

   m_fragments.push_back(std::move(fragment));
   return;
}

void Event::clear() {

   m_fragments.clear();
   return;
}

/**
 * @returns The estimated size of the serialized event in bytes
 */
uint32_t Event::sizeInBytes() const {

   return std::for_each(m_fragments.begin(), m_fragments.end(), CountBytes())
       .size();
}

}  // namespace ev
