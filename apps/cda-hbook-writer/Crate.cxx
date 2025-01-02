//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// CDA include(s):
#include "common/errorcheck.h"
#include "device/Factory.h"

// Local include(s):
#include "Crate.h"

namespace hbook {

Crate::Crate()
    : dev::Crate<dev::ICernlibDisk>(), m_nmgr(), m_logger("hbook::Crate") {}

StatusCode Crate::initialize(const QString& fileName) {

   //
   // Create the output variables of each device:
   //
   for (auto& device : m_devices) {
      CHECK(device.second->initialize(m_nmgr));
   }
   // Open the output file.
   CHECK(m_nmgr.openFile(fileName));

   // Return gracefully.
   return StatusCode::SUCCESS;
}

StatusCode Crate::writeEvent(const ev::Event& event) {

   // Loop over the fragments coming from the different modules that
   // are used in data acquisition:
   for (auto& fragment : event.getFragments()) {

      // Find the device that is expecting this event fragment:
      DeviceMap_t::iterator device = m_devices.find(fragment->getModuleID());
      if (device == m_devices.end()) {
         REPORT_ERROR(tr("Failed to assign fragment with "
                         "module ID: %1")
                          .arg(fragment->getModuleID()));
         return StatusCode::FAILURE;
      }

      // Give this fragment to the device that we just found:
      CHECK(device->second->writeEvent(*fragment, m_nmgr));
   }

   // Persistify the current event:
   CHECK(m_nmgr.saveEvent());

   // Return gracefully.
   return StatusCode::SUCCESS;
}

StatusCode Crate::finalize() {

   m_nmgr.closeFile();
   return StatusCode::SUCCESS;
}

}  // namespace hbook
