// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_FRAGMENT_H
#define CDA_CORE_EVENT_FRAGMENT_H

// System include(s):
#include <stdint.h>

// STL include(s):
#include <vector>

/**
 *  @short Namespace for the event classes
 *
 *         This namespace holds all the classes which are used to
 *         transmit the readout information from the CAMAC crate(s)
 *         to the applications processing this data.
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
    *         Each CAMAC device is supposed to fill one such object.
    *         The fragment can identify which slot (in which crate)
    *         it was coming from, and carries a free-format vector
    *         of data words. It's up to the device plugins to handle
    *         the data words consistently. (The different devices
    *         are allowed to use the data words completely differently.)
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Fragment {

   public:
      /// Default constructor
      Fragment();

      /// Get the crate slot in which the module is put
      int getModuleID() const;
      /// Get the data payload of the module
      const std::vector< uint32_t >& getDataWords() const;

      /// Set the crate slot in which the module is put
      void setModuleID( int value );
      /// Add a 32-bit data word (information from one channel)
      void addDataWord( uint32_t dataWord );

      /// Clear the event fragment
      void clear();

   private:
      int m_moduleNumber; ///< Module number in the CAMAC crate
      std::vector< uint32_t > m_dataWords; ///< Data from the module

   }; // class Fragment

} // namespace ev

#endif // CDA_CORE_EVENT_FRAGMENT_H
