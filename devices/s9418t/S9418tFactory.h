// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_S9418T_S9418TFACTORY_H
#define CDA_DEVICES_S9418T_S9418TFACTORY_H

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
 *  @short Namespace for the Silena 9418T TDC device
 *
 *         This namespace holds all the classes that handle the a
 *         Silena 9418/T VME TDC device.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace s9418t {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QObject );
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Factory creating all the Silena9418T objects
    *
    *         This factory is used when loading the Silena9418T plugin to access
    *         all the classes provided by the plugin.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class S9418tFactory : public QObject,
                         virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
      Q_PLUGIN_METADATA( IID "hu.atomki.CDA.dev.s9418t/1.0" )
#endif

   public:
      /// Give the short name of the device implemented in the plug-in
      virtual QString shortName() const;
      /// Give the long name of the device implemented in the plug-in
      virtual QString longName() const;

      /// Universal function for creating a device object of this type
      virtual void* createDevice( const std::type_info& ti ) const;

   }; // class S9418tFactory

} // namespace s9418t

#endif // CDA_DEVICES_S9418T_S9418TFACTORY_H
