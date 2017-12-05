// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_EVENT_INCOMPLETEEVENT_H
#define CDA_CORE_EVENT_INCOMPLETEEVENT_H

// STL include(s):
#include <exception>

namespace ev {

   /**
    *  @short Incomplete event exception
    *
    *         With the events becoming larger and larger, it is now possible
    *         to stop an event sending application while it is sending its
    *         data. When this happens, the reading application needs to
    *         acknowledge that it received an incomplete event, and do
    *         something with this information. (Probably drop the event.)
    *
    *         When such an incomplete event is received, this type
    *         of exception is thrown by the event server.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class IncompleteEvent : public std::exception {

   public:
      /// Give a description of the exception
      const char* what() const throw() {
         return "Incomplete event received";
      }

   }; // class IncompleteEvent

} // namespace ev

#endif // CDA_CORE_EVENT_INCOMPLETEEVENT_H
