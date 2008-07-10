// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CRATE_H
#define CDA_CORE_DEVICE_CRATE_H

// STL include(s):
#include <map>

// CDA include(s):
#include "../msg/Logger.h"

// Local include(s):
#include "Config.h"
#include "Factory.h"

namespace dev {

   // Forward declaration(s):
   class Loader;

   /**
    *  @short Base class for all CAMAC crate classes
    *
    *         All the CDA applications have to handle a crate of CAMAC
    *         devices. The configuration of these devices is always
    *         read and written in the same way, so it makes sense to only
    *         code it once. The situation is complicated a bit by the
    *         fact that all crate classes have to handle different types
    *         of modules, but it's nothing a little template implementation
    *         couldn't handle.
    *
    *         The class heavily uses dev::Factory for creating the
    *         device objects when reading a configuration. Since dev::Factory
    *         provides multiple functions for creating different kinds
    *         of devices, you have to specify in the constructor which
    *         of these functions should be used by this class. This is
    *         done by passing the constructor the proper function
    *         pointer. A bit fancy, but it works...
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   template< class Device >
   class Crate : public virtual Config {

   public:
      /// Constructor
      Crate( Device* ( dev::Factory::*factoryFncn )( void ) const );
      /// Destructor
      ~Crate();

      /// Read the configuration of the crate from a binary device
      virtual bool readConfig( QIODevice* dev );
      /// Write the configuration of the crate to a binary device
      virtual bool writeConfig( QIODevice* dev ) const;

      /// Read the configuration of the crate from an XML node
      virtual bool readConfig( const QDomNode& node );
      /// Write the configuration of the crate to an XML node
      virtual bool writeConfig( QDomNode& node ) const;

      /// Get the dev::Loader object used by the crate
      Loader* getLoader() const;
      /// Set the dev::Loader object used by the crate
      void setLoader( const Loader* loader );

      /// Clear the configuration of the crate
      void clear();

   protected:
      /// Maximal number of slots the devices can use
      static const int NUMBER_OF_SLOTS = 24;
      /// Container for the devices
      /**
       * I'm not yet sure about this std::map... A simple array of
       * Device pointers could be used just as well I guess. It works
       * for the moment, but might be considered too fancy later on.
       */
      std::map< int, Device* > m_devices;

   private:
      /// Check that a usable dev::Loader is set
      bool checkLoader() const;

      /// Function in dev::Factory that is used to create new devices
      Device* ( dev::Factory::*m_factoryFncn )( void ) const;
      /// The dev::Loader class used by the class
      const Loader* m_loader;

      /// Message logging object
      mutable msg::Logger m_logger;

   }; // class Crate

} // namespace dev

// Include the implementation:
#include "Crate.icc"

#endif // CDA_CORE_DEVICE_CRATE_H
