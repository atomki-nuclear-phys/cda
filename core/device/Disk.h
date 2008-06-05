// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_DISK_H
#define CDA_CORE_DEVICE_DISK_H

// Local include(s):
#include "Config.h"

// Forward include(s):
namespace ev {
   class Fragment;
} // namespace ev

namespace dev {

   /**
    *  @short Interface for writing an output file from the readout data
    *
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Disk : virtual public Config {

   public:
      /// Function initializing the device
      virtual void initialize() = 0;
      /// Function filling the histograms
      virtual void writeEvent( const ev::Fragment& fragment ) = 0;

   }; // class Disk

} // namespace dev

#endif // CDA_CORE_DEVICE_DISK_H
