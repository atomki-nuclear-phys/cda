// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_READOUT_H
#define CDA_DEVICES_EXAMPLE_READOUT_H

// Qt include(s):
#include <QtCore/QGlobalStatic>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Readout.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Readout.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Config.h"

namespace camac {

   class Readout : virtual public dev::Readout,
                   virtual public Config {

   public:
      Readout();

      virtual bool initialize() const;
      virtual ev::Fragment readEvent() const;

   private:
      mutable msg::Logger m_logger;

   }; // class Readout

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_READOUT_H