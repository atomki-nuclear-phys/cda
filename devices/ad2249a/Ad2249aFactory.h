// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD2249A_AD2249AFACTORY_H
#define CDA_DEVICES_AD2249A_AD2249AFACTORY_H

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
 *  @short Namespace for the AD2249A ADC device
 *
 *         This namespace holds all the classes that handle the a AD2249A
 *         CAMAC ADC device.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace ad2249a {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QObject );
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Factory creating all the AD2249A objects
    *
    *         This factory is used when loading the AD2249A plugin to access
    *         all the classes provided by the plugin.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Ad2249aFactory : public QObject,
                          virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
      Q_PLUGIN_METADATA( IID "hu.atomki.CDA.dev.ad2249/1.0" )
#endif

   public:
      /// Give the short name of the device implemented in the plug-in
      virtual QString shortName() const;
      /// Give the long name of the device implemented in the plug-in
      virtual QString longName() const;

      /// Universal function for creating a device object of this type
      virtual void* createDevice( const std::type_info& ti ) const;

   }; // class Ad2249aFactory

} // namespace ad2249a

#endif // CDA_DEVICES_AD2249A_AD2249AFACTORY_H