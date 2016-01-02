// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CRATE_H
#define CDA_CORE_DEVICE_CRATE_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QCoreApplication>
#include <QtCore/QString>

// CDA include(s):
#include "../msg/Logger.h"

// Local include(s):
#include "IConfig.h"
#include "Factory.h"
#include "../common/UniquePtr.h"

namespace dev {

   // Forward declaration(s):
   class Loader;

   /**
    *  @short Base class for all CAMAC/VME/CAEN "crate" classes
    *
    *         All the CDA applications have to handle a "crate" of CAMAC/VME/CAEN
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
   template< class DEVICE >
   class Crate : public virtual IConfig {

      Q_DECLARE_TR_FUNCTIONS( dev::Crate )

   public:
      /// Constructor
      Crate( const QString& type = "CamacCrate",
             bool isTypeImportant = false );
      /// Destructor
      ~Crate();

      /// Read the configuration of the crate from a binary device
      virtual bool readConfig( QIODevice& dev );
      /// Write the configuration of the crate to a binary device
      virtual bool writeConfig( QIODevice& dev ) const;

      /// Read the configuration of the crate from an XML node
      virtual bool readConfig( const QDomElement& node );
      /// Write the configuration of the crate to an XML node
      virtual bool writeConfig( QDomElement& node ) const;

      /// Check if this object can read this binary configuration
      bool canRead( QIODevice& dev ) const;
      /// Check if this object can read this XML configuration
      bool canRead( const QDomElement& node ) const;

      /// Get the dev::Loader object used by the crate
      const Loader* getLoader() const;
      /// Set the dev::Loader object used by the crate
      void setLoader( const Loader* loader );

      /// Clear the configuration of the crate
      virtual void clear();

   protected:
      /// Check that a usable dev::Loader is set
      virtual bool checkLoader() const;

      /// Read the crate specific options from binary input
      virtual bool readCrateConfig( QIODevice& dev );
      /// Write the crate specific options to binary output
      virtual bool writeCrateConfig( QIODevice& dev ) const;

      /// Read the crate specific options from XML input
      virtual bool readCrateConfig( const QDomElement& node );
      /// Write the crate specific options to XML output
      virtual bool writeCrateConfig( QDomElement& node ) const;

      /// Type of the device container
      typedef std::map< unsigned int,
                        typename UniquePtr< DEVICE >::Type > DeviceMap_t;

      /// Container for the devices
      /**
       * The key is the "ID" of the device. In case of a CAMAC crate this
       * is the slot number of the device, in case of VME, the address.
       * In the CAEN case we only have one device, so the key is not too
       * important...
       */
      DeviceMap_t m_devices;

      /// XML node name for the crate
      static const char* XML_NODE_NAME;

      /// The dev::Loader class used by the class
      const Loader* m_loader;

   private:
      /// Type of this specific crate
      const QString m_type;
      /// Is the type of the configuration important?
      const bool m_isTypeImportant;
      /// Message logging object
      mutable msg::Logger m_logger;

   }; // class Crate

} // namespace dev

// Include the implementation:
#include "Crate.icc"

#endif // CDA_CORE_DEVICE_CRATE_H
