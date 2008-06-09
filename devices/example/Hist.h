// Dear emacs, this is -*- c++ -*-
#ifndef CDA_DEVICES_EXAMPLE_HIST_H
#define CDA_DEVICES_EXAMPLE_HIST_H

// CDA include(s):
#include "device/Hist.h"
#include "msg/Logger.h"

// Local include(s):
#include "Config.h"

namespace camac {

   class Hist : public virtual dev::Hist,
                public virtual Config {

   public:
      Hist();

      virtual bool initialize();
      virtual bool displayEvent( const ev::Fragment& fragment );

   private:
      mutable msg::Logger m_logger;

   }; // class Hist

} // namespace camac

#endif // CDA_DEVICES_EXAMPLE_HIST_H
