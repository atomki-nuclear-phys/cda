// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_HIST_H
#define CDA_CORE_DEVICE_HIST_H

// Local include(s):
#include "Config.h"

// Forward include(s):
namespace ev {
   class Fragment;
} // namespace ev

namespace dev {

   /**
    *  @short Interface for creating histograms from the readout data
    *
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Hist : virtual public Config {

   public:
      /// Function initializing the device
      virtual void initialize() = 0;
      /// Function filling the histograms
      virtual void displayEvent( const ev::Fragment& fragment ) = 0;

   }; // class Hist

} // namespace dev

#endif // CDA_CORE_DEVICE_HIST_H
