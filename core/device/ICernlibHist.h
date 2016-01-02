// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICE_CERNLIBHIST_H
#define CDA_CORE_DEVICE_CERNLIBHIST_H

// Local include(s):
#include "IDevice.h"

// Forward declaration(s):
namespace cernlib {
   class HistMgr;
} // namespace cernlib
namespace ev {
   class Fragment;
} // namespace ev

namespace dev {

   /**
    *  @short Interface for creating histograms from the readout data
    *
    *         This class describes how devices can fill monitoring histograms
    *         with their readout data.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class ICernlibHist : virtual public IDevice {

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
       * @param hmgr Higtogram manager to be used for booking the histograms
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool initialize( cernlib::HistMgr& hmgr ) = 0;
      /// Function filling the histograms
      /**
       * Fill the histograms with an event read from this device.
       * This is of course only a first guess of how this function
       * should look like.
       *
       * @param fragment Event fragment that should be processed
       * @param hmgr Histogram manager to fill the histograms with
       * @returns <code>true</code> if the operation was successful,
       *          <code>false</code> otherwise
       */
      virtual bool displayEvent( const ev::Fragment& fragment,
                                 const cernlib::HistMgr& hmgr ) const = 0;

   }; // class ICernlibHist

} // namespace dev

#endif // CDA_CORE_DEVICE_ICERNLIBHIST_H
