
// STL include(s):
#include <functional>
#include <algorithm>

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
   class CountBytes :
      public std::unary_function< ev::Event::Base_t::value_type, void > {

   public:
      /// Constructor initializing the event size variable
      CountBytes()
         : std::unary_function< ev::Event::Base_t::value_type, void >(),
           m_size( 0 ) {

      }

      /// Add the size of one event fragment
      result_type operator() ( const argument_type& obj ) {
         m_size += obj->sizeInBytes();
         return;
      }

      /// Get the calculated size of the event in bytes
      uint32_t size() const {
         return m_size;
      }

   private:
      uint32_t m_size; ///< Collective size of the event

   }; // class CountBytes

} // private namespace

namespace ev {

   Event::Event()
      : Event::Base_t() {

   }

   Event::Event( const Event& parent )
      : Event::Base_t( parent ) {

   }

   Event& Event::operator= ( const Event& rh ) {

      Base_t::operator=( rh );

      return *this;
   }

   /**
    * This is a backwards compatibility function. Should be removed at one
    * point, once all the clients have moved to the new interface.
    *
    * @returns The payload of the event
    */
   const Event::Base_t& Event::getFragments() const {

      return *this;
   }

   /**
    * This is a backwards compatibility function. Should be removed at one
    * point, once all the clients have moved to the new interface.
    *
    * @param fragment The event fragment to be added to the event
    */
   void Event::addFragment( std::unique_ptr< Fragment > fragment ) {

      Base_t::push_back( std::move( fragment ) );
      return;
   }

   /**
    * @param fragment The event fragment to be added to the event
    */
   void Event::push_back( std::unique_ptr< Fragment > fragment ) {

      Base_t::push_back( std::move( fragment ) );
      return;
   }

   /**
    * @returns The estimated size of the serialized event in bytes
    */
   uint32_t Event::sizeInBytes() const {

      return std::for_each( begin(), end(), CountBytes() ).size();
   }

} // namespace ev
