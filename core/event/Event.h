// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_EVENT_H
#define CDA_CORE_EVENT_EVENT_H

// STL include(s):
#include <vector>
#include <memory>

// Local include(s):
#include "Fragment.h"
#include "../common/UniquePtr.h"
#include "../common/SharedPtr.h"

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
    *
    * $Revision$
    * $Date$
    */
   class Event :
      public std::vector< SharedPtr< Fragment >::Type > {

   public:
      /// Type of the base class
      typedef std::vector< SharedPtr< Fragment >::Type > Base_t;

      /// Default constructor
      Event();
      /// Copy constructor
      Event( const Event& parent );

      /// Copy operator
      Event& operator= ( const Event& rh );

      /// Get all the event fragments
      const Base_t& getFragments() const;
      /// Add one more event fragment
      void addFragment( UniquePtr< Fragment >::Type fragment );
      /// Add one more event fragment
      void push_back( UniquePtr< Fragment >::Type fragment );

      /// Get the size of this event in bytes
      uint32_t sizeInBytes() const;

   }; // class Event

} // namespace ev

#endif // CDA_CORE_EVENT_EVENT_H
