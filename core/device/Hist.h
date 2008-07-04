// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_HIST_H
#define CDA_CORE_DEVICE_HIST_H

// Local include(s):
#include "Device.h"

// Forward declaration(s):
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
   class Hist : virtual public Device {

   public:
      /// Function initializing the device
      /**
       * Prepare the writing of histograms with data read from this
       * device. The histograms are numbered starting from 1. In order
       * not to have clashes between the different devices, a "counter"
       * argument is required by this function that tells it which
       * histogram numbers are still available. The function is required
       * to increment this variable by as much as many histograms it
       * created.
       *
       * @param counter Counter where to start numbering histograms from
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( unsigned int& counter ) = 0;
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
