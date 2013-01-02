// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T4300B_FACTORY_H
#define CDA_DEVICES_T4300B_FACTORY_H

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
 *  @short Namespace for the T4300B QDC device
 *
 *         This namespace holds all the classes that handle the a T4300B
 *         CAMAC QDC device.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace t4300b {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QObject );
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Factory creating all the T4300B objects
    *
    *         This factory is used when loading the T4300B plugin to access
    *         all the classes provided by the plugin.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Factory : public QObject,
                   virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
      Q_PLUGIN_METADATA( IID "hu.atomki.CDA.dev.t4300b/1.0" )
#endif

   public:
      /// Get the shortened name of the device
      virtual QString shortName() const;
      /// Get the long name of the device
      virtual QString longName() const;

      /// Universal function for creating a device object of this type
      virtual void* createDevice( const std::type_info& ti ) const;

   }; // class Factory

} // namespace t4300b

#endif // CDA_DEVICES_T4300B_FACTORY_H
