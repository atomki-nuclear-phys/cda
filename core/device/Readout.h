// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_READOUT_H
#define CDA_CORE_DEVICE_READOUT_H

// CDA include(s):
#include "event/Fragment.h"

// Local include(s):
#include "Config.h"

namespace dev {

   /**
    *  @short Interface for reading out data from a CAMAC device
    *
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Readout : virtual public Config {

   public:
      /// Function initializing the device
      virtual void initialize() const = 0;
      /// Function reading the current data from the device
      virtual ev::Fragment readEvent() const = 0;

   }; // class Readout

} // namespace dev

#endif // CDA_CORE_DEVICE_READOUT_H
