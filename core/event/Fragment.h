// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_FRAGMENT_H
#define CDA_CORE_EVENT_FRAGMENT_H

// System include(s):
#include <stdint.h>

// STL include(s):
#include <vector>

// CDA include(s):
#include "../common/Export.h"

/**
 *  @short Namespace for the event classes
 *
 *         This namespace holds all the classes which are used to
 *         transmit the readout information from the readout applications
 *         communicating with the hardwars, to the applications processing
 *         data.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace ev {

   /**
    *  @short Basic data fragment coming from one device
    *
    *         Each CAMAC/CAEN/VME device is supposed to fill one such object.
    *         The fragment can identify itself with an ID specific
    *         to the device, and carries a free-format vector
    *         of data words. It's up to the device plugins to handle
    *         the data words consistently. (The different devices
    *         are allowed to use the data words completely differently.)
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT Fragment {

   public:
      /// Type of the module identifier
      typedef uint32_t Identifier_t;
      /// Type of the stored information
      typedef std::vector< uint32_t > Payload_t;

      /// Default constructor
      Fragment( Identifier_t moduleID = 0xffffffff );

      /// Get the crate slot in which the module is put
      Identifier_t getModuleID() const;
      /// Get the data payload of the module
      const Payload_t& getDataWords() const;

      /// Set the crate slot in which the module is put
      void setModuleID( Identifier_t value );
      /// Add a 32-bit data word (information from one channel)
      void addDataWord( Payload_t::value_type dataWord );

      /// Get the size of this data fragment in bytes
      uint32_t sizeInBytes() const;

      /// Clear the event fragment
      void clear( bool clearModuleID = true );

   private:
      Identifier_t m_moduleID;  ///< ID of event fragment source
      Payload_t    m_dataWords; ///< Data from the module

   }; // class Fragment

} // namespace ev

#endif // CDA_CORE_EVENT_FRAGMENT_H
