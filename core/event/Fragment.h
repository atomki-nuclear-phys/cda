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
      /// Defailt constructor
      Fragment();
      Fragment(int);
      /// Destructor
      ~Fragment();

      /// Get the number of the crate in which the module is
      int getCrateNumber() const;
      /// Get the crate slot in which the module is put
      int getModuleNumber() const;
      /// Get the data payload of the module
      const std::vector< uint32_t >& getDataWords() const;

      /// Set the numner of the crate in which the module is
      void setCrateNumber( int value );
      /// Set the crate slot in which the module is put
      void setModuleNumber( int value );
      /// Add a 32-bit data word (information from one channel)
      void addDataWord( uint32_t dataWord );

      /// Clear the event fragment
      void clear();

   private:
      int m_crateNumber; ///< CAMAC crate number
      int m_moduleNumber; ///< Module number in the CAMAC crate
      std::vector< uint32_t > m_dataWords; ///< Data from the module

   }; // class Fragment

} // namespace ev

#endif // CDA_CORE_EVENT_FRAGMENT_H
