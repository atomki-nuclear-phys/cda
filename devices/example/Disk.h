// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_DISK_H
#define CDA_DEVICES_EXAMPLE_DISK_H

// Qt include(s):
#include <QtCore/QGlobalStatic>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Disk.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Disk.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Config.h"

namespace camac {

   class Disk : public virtual dev::Disk,
                public virtual Config {

   public:
      Disk();

      virtual bool initialize();
      virtual bool writeEvent( const ev::Fragment& fragment );

   private:
      mutable msg::Logger m_logger;

   }; // class Disk

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_DISK_H
