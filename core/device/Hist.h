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
    *         This class is similar in function to dev::Disk. It is used
    *         to instruct the histogram writing objects.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Hist : virtual public Config {

   public:
      /// Function initializing the device
      /**
       * Prepare the writing of histograms with data read from this
       * device.
       *
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize() = 0;
      /// Function filling the histograms
      /**
       * Fill the histograms with an event read from this device.
       * This is of course only a first guess of how this function
       * should look like.
       *
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool displayEvent( const ev::Fragment& fragment ) = 0;

   }; // class Hist

} // namespace dev

#endif // CDA_CORE_DEVICE_HIST_H
