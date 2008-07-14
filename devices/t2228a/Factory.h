// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_FACTORY_H
#define CDA_DEVICES_EXAMPLE_FACTORY_H

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
      Factory();

      virtual QString shortName() const;
      virtual QString longName() const;

      virtual dev::Gui*     createGui() const;
      virtual dev::Readout* createReadout() const;
      virtual dev::Hist*    createHist() const;
      virtual dev::Disk*    createDisk() const;

   }; // class Factory

} // namespace t2228a

#endif // CDA_DEVICES_EXAMPLE_FACTORY_H
