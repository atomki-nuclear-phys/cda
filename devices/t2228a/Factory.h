// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_T2228A_FACTORY_H
#define CDA_DEVICES_T2228A_FACTORY_H

// Qt include(s):
#include <QtCore/QObject>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Factory.h"
#   include "msg/Logger.h"
#endif

// There is no bloody way to tell qmake to add a -F... flag when running
// MOC, so when MOC is being run on Mac OS X, this include has to be used...
#ifdef Q_MOC_RUN
#   include "device/Factory.h"
#endif

/**
 *  @short Namespace for the T2228A TDC device
 *
 *         This namespace holds all the classes that handle the a T2228A
 *         CAMAC TDC device.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace t2228a {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QObject );
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Factory creating all the T2228A objects
    *
    *         This factory is used when loading the T2228A plugin to access
    *         all the classes provided by the plugin.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Factory : public QObject,
                   virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )

   public:
      /// Give the short name of the device implemented in the plug-in
      /**
       * Each device type has to provide a short, unique string that
       * identifies it. This text is used in the configuration files
       * (both binary and XML) to identify the devices.
       *
       * @returns The short name of the device
       */
      virtual QString shortName() const;
      /// Give the long name of the device implemented in the plug-in
      /**
       * Besides the short names, the devices should also provide a longer,
       * more descriptive name for the devices. These are only used to
       * print messages and in GUIs, so they don't have to be too compact,
       * or contain only one word.
       *
       * @returns The long name of the device
       */
      virtual QString longName() const;

      /// Universal function for creating a device object of this type
      virtual void* createDevice( const std::type_info& ti ) const;

   }; // class Factory

} // namespace t2228a

#endif // CDA_DEVICES_T2228A_FACTORY_H
