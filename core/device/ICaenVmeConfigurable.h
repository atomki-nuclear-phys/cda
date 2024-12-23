// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_DEVICE_ICAENVMECONFIGURABLE_H
#define CDA_CORE_DEVICE_ICAENVMECONFIGURABLE_H

// Local include(s).
#include "IDevice.h"

// Forward declaration(s).
namespace caen {
class VmeBus;
}

namespace dev {

/**
 *  @short Base interface for all CAEN VME "configurable" devices
 *
 *         This base class describes devices that can be "configured".
 *         These devices don't take part in the data collection, they
 *         are just physically placed into the same crate as some other
 *         devices, and need to be set up (configured) correctly for the
 *         data taking to work.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class ICaenVmeConfigurable : public virtual IDevice {

public:
   /// Configure the device for data taking
   /**
    * This function needs to be called to apply the configuration read in
    * from a file, to the physical device.
    *
    * @return The usual @c StatusCode types
    */
   virtual StatusCode configure(const caen::VmeBus& bus) const = 0;

};  // class ICaenVmeConfigurable

}  // namespace dev

#endif  // CDA_CORE_DEVICE_ICAENVMECONFIGURABLE_H
