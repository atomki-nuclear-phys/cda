// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_EVENT_EVENT_H
#define CDA_CORE_EVENT_EVENT_H

// STL include(s):
#include <vector>
#include <memory>

// CDA include(s):
#include "../common/Export.h"

// Local include(s):
#include "Fragment.h"

namespace ev {

   /**
    *  @short A whole event read out during data taking
    *
    *         This class represents a whole event read out from one of
    *         the data sources. (CAMAC/CAEN/VME) It is a collection of
    *         ev::Fragment objects that each describe the information
    *         coming from one device.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDACORE_EXPORT Event {

   public:
      /// Type of the base class
      typedef std::vector< std::unique_ptr< Fragment > > Fragments_t;

      /// Default constructor
      Event() = default;
      /// Move constructor
      Event( Event&& ) = default;

      /// Move operator
      Event& operator= ( Event&& ) = default;

      /// Get all the event fragments
      const Fragments_t& getFragments() const;
      /// Add one more event fragment
      void addFragment( std::unique_ptr< Fragment > fragment );

      /// Clear the event of all its fragments
      void clear();

      /// Get the size of this event in bytes
      uint32_t sizeInBytes() const;

   private:
      /// Event fragments
      Fragments_t m_fragments;

   }; // class Event

} // namespace ev

#endif // CDA_CORE_EVENT_EVENT_H
