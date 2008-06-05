// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_FACTORY_H
#define CDA_CORE_DEVICE_FACTORY_H

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

   // Forward declaration(s):
   class Gui;
   class Readout;
   class Hist;
   class Disk;

   /**
    *  @short Factory that creates all kinds of objects for a device type
    *
    *         Each device will be built in its own plug-in. Qt is able to
    *         instantiate such a factory after loading the plugin. The
    *         factory is then used to create new instances of different
    *         classes handling the same device type.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Factory {

   public:
      /// Destructor
      virtual ~Factory() {}

      /// Give the name of the device implemented in the plug-in
      /**
       * To do some book-keeping of the devices that are available,
       * each factory has to report what kind of device it handles.
       * This string will then be used to list the available devices in
       * the GUI and so the framework would know which factory to use
       * when reading a data acquisition configuration.
       */
      virtual QString deviceName() const = 0;

      /// Create a graphical class describing the device
      virtual Gui*     createGui( QWidget* widget = 0,
                                  Qt::WindowFlags flags = 0 ) const = 0;
      /// Create a readout class handling the device
      virtual Readout* createReadout() const = 0;
      /// Create an object writing histograms with the device's data
      virtual Hist*    createHist() const = 0;
      /// Create an object writing an event file with the device's data
      virtual Disk*    createDisk() const = 0;

   }; // class Factory

} // namespace dev

Q_DECLARE_INTERFACE( dev::Factory, "hu.atomki.CDA.dev.Factory/0.0.1" )

#endif // CDA_CORE_DEVICE_FACTORY_H
