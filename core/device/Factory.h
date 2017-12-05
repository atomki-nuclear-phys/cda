// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_FACTORY_H
#define CDA_CORE_DEVICE_FACTORY_H

// System include(s):
#include <typeinfo>
#include <memory>

// Qt include(s):
#include <QtCore/QObject>
#include <QtCore/QString>

namespace dev {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QObject );

   /**
    *  @short Factory that creates all kinds of objects for a device type
    *
    *         Each device will be built in its own plug-in. Qt is able to
    *         instantiate such a factory after loading the plugin. The
    *         factory is then used to create new instances of different
    *         classes handling the same device type.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class Factory {

   public:
      /// Destructor
      virtual ~Factory() {}

      /// Give the short name of the device implemented in the plug-in
      /**
       * Each device type has to provide a short, unique string that
       * identifies it. This text is used in the configuration files
       * (both binary and XML) to identify the devices.
       *
       * @returns The short name of the device
       */
      virtual const QString& shortName() const = 0;

      /// Give the long name of the device implemented in the plug-in
      /**
       * Besides the short names, the devices should also provide a longer,
       * more descriptive name for the devices. These are only used to
       * print messages and in GUIs, so they don't have to be too compact,
       * or contain only one word.
       *
       * @returns The long name of the device
       */
      virtual const QString& longName() const = 0;

      /// Helper function for creating a new device of a given type
      template< class T >
      std::unique_ptr< T > createDevice() const;

   protected:
      /// Universal function for creating a device object of this type
      /**
       * All the plugins are supposed to provide their different device
       * objects through this single interface now.
       *
       * @param ti The type info of the device being requested
       * @returns A typeless pointer to one object of the requested type
       */
      virtual void* createDevice( const std::type_info& ti ) const = 0;

   }; // class Factory

} // namespace dev

// Include the template implementation(s):
#include "Factory.icc"

// Declare the dev::Factory interface to Qt:
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE( dev::Factory, "hu.atomki.CDA.dev.Factory/1.1.0" )
QT_END_NAMESPACE

#endif // CDA_CORE_DEVICE_FACTORY_H
