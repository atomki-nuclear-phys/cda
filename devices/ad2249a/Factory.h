// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_AD2249A_FACTORY_H
#define CDA_DEVICES_AD2249A_FACTORY_H

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
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision: 39 $
 * $Date: 2008-07-14 19:15:52 +0200 (Mon, 14 Jul 2008) $
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
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision: 39 $
    * $Date: 2008-07-14 19:15:52 +0200 (Mon, 14 Jul 2008) $
    */
   class Factory : public QObject,
                   virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )

   public:
      Factory();

      virtual QString shortName() const;
      virtual QString longName() const;

      virtual dev::Gui*     createGui() const;
      virtual dev::Readout* createReadout() const;
      virtual dev::Hist*    createHist() const;
      virtual dev::Disk*    createDisk() const;

   }; // class Factory

} // namespace ad2249a

#endif // CDA_DEVICES_AD2249A_FACTORY_H