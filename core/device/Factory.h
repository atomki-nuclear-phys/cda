// Dear emacs, this is -*- c++ -*-
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

   class Factory {

   public:
      virtual ~Factory() {}

      virtual QString deviceName() const = 0;

      virtual Gui*     createGui( QWidget* widget = 0,
                                  Qt::WindowFlags flags = 0 ) const = 0;
      virtual Readout* createReadout() const = 0;
      virtual Hist*    createHist() const = 0;
      virtual Disk*    createDisk() const = 0;

   }; // class Factory

} // namespace dev

Q_DECLARE_INTERFACE( dev::Factory, "hu.atomki.CDA.dev.Factory/0.0.1" )

#endif // CDA_CORE_DEVICE_FACTORY_H
