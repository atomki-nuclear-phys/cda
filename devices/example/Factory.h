// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_FACTORY_H
#define CDA_DEVICES_EXAMPLE_FACTORY_H

// Qt include(s):
#include <QtCore/QObject>

// CDA include(s):
#include "device/Factory.h"
#include "msg/Logger.h"

namespace camac {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QObject );
   using QT_PREPEND_NAMESPACE( QString );

   class Factory : public QObject,
                   virtual public dev::Factory {

      Q_OBJECT
      Q_INTERFACES( dev::Factory )

   public:
      Factory();

      virtual QString deviceName() const;

      virtual dev::Gui*     createGui( QWidget* parent = 0,
                                       Qt::WindowFlags flags = 0 ) const;
      virtual dev::Readout* createReadout() const;
      virtual dev::Hist*    createHist() const;
      virtual dev::Disk*    createDisk() const;

   private:
      mutable msg::Logger m_logger;

   }; // class Factory

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_FACTORY_H
