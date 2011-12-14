// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_EVENT_H
#define CDA_CORE_EVENT_EVENT_H

// STL include(s):
#include <vector>
#include <tr1/memory>

// Local include(s):
#include "Fragment.h"

namespace ev {

   /**
    *  @short A whole event read out from the CAMAC system
    *
    *         This class represents a whole event read out from the CAMAC
    *         system. It is a collection of ev::Fragment objects that each
    *         describe the information coming from one device.
    *
    *         In theory this Event Data Model (EDM) can handle multiple
    *         CAMAC crates with readout modules. However other parts of the
    *         system are not likely to ever support this...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Event {

   public:
      /// Default constructor
      Event();
      /// Copy constructor
      Event( const Event& parent );

      /// Copy operator
      Event& operator= ( const Event& rh );

      /// Get all the event fragments
      const std::vector< std::tr1::shared_ptr< Fragment > >&
      getFragments() const;
      /// Add one more event fragment
      void addFragment( Fragment* fragment );

      /// Clear the event object
      void clear();

   private:
      /// Event fragments (module info)
      std::vector< std::tr1::shared_ptr< Fragment > > m_fragments;

   }; // class Event

} // namespace ev

#endif // CDA_CORE_EVENT_EVENT_H
