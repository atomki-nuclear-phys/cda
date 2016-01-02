// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_AD413AFACTORY_H
#define CDA_DEVICES_AD413A_AD413AFACTORY_H

// Qt include(s):
#include <QtCore/QObject>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Factory.h"
#else
#   include "device/Factory.h"
#endif

// There is no bloody way to tell qmake to add a -F... flag when running
// MOC, so when MOC is being run on Mac OS X, this include has to be used...
#ifdef Q_MOC_RUN
#   include "device/Factory.h"
#endif

/**
 *  @short Namespace for the AD413A ADC device
 *
 *         This namespace holds all the classes that handle the an AD413A
 *         CAMAC ADC device.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace ad413a {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QObject );
   using QT_PREPEND_NAMESPACE( QString );

   /**
    *  @short Factory creating all the AD413A objects
    *
    *         This factory is used when loading the AD413A plugin to access
    *         all the classes provided by the plugin.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Ad413aFactory : public QObject,
                         virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
      Q_PLUGIN_METADATA( IID "hu.atomki.CDA.dev.ad413a/1.0" )
#endif

   public:
      /// Give the short name of the device implemented in the plug-in
      virtual const QString& shortName() const;
      /// Give the long name of the device implemented in the plug-in
      virtual const QString& longName() const;

   protected:
      /// Universal function for creating a device object of this type
      virtual void* createDevice( const std::type_info& ti ) const;

   }; // class Ad413aFactory

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_AD413AFACTORY_H
