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

      /// Give the short name of the device implemented in the plug-in
      /**
       * Each device type has to provide a short, unique string that
       * identifies it. This text is used in the configuration files
       * (both binary and XML) to identify the devices.
       *
       * @returns The short name of the device
       */
      virtual QString shortName() const = 0;

      /// Give the long name of the device implemented in the plug-in
      /**
       * Besides the short names, the devices should also provide a longer,
       * more descriptive name for the devices. These are only used to
       * print messages and in GUIs, so they don't have to be too compact,
       * or contain only one word.
       *
       * @returns The long name of the device
       */
      virtual QString longName() const = 0;

      /// Create a graphical class describing the device
      virtual Gui*     createGui() const = 0;
      /// Create a readout class handling the device
      virtual Readout* createReadout() const = 0;
      /// Create an object writing histograms with the device's data
      virtual Hist*    createHist() const = 0;
      /// Create an object writing an event file with the device's data
      virtual Disk*    createDisk() const = 0;

   }; // class Factory

} // namespace dev

// Declare the dev::Factory interface to Qt:
Q_DECLARE_INTERFACE( dev::Factory, "hu.atomki.CDA.dev.Factory/0.0.1" )

#endif // CDA_CORE_DEVICE_FACTORY_H
