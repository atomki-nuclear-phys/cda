// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_FRAGMENT_H
#define CDA_CORE_EVENT_FRAGMENT_H

/**
 *  @short Namespace for the event classes
 *
 *         This namespace is just an idea for the moment...
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace ev {

   /**
    *  @short Basic data fragment coming from one CAMAC device
    *
    *         This is nothing serious for the time being.
    *         Just something to fill the space for now. My idea
    *         is that events read from the CAMAC devices should
    *         be placed in proper OO structures later on to be
    *         able to pass them around between the executables.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Fragment {

   public:
      Fragment();
      ~Fragment();

   }; // class Fragment

} // namespace ev

#endif // CDA_CORE_EVENT_FRAGMENT_H
