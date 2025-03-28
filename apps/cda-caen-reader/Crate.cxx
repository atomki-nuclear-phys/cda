// $Id$

// CDA include(s):
#include "caen/Digitizer.h"
#include "common/errorcheck.h"

// Local include(s):
#include "Crate.h"

namespace caen_reader {

Crate::Crate()
    : dev::Crate<dev::ICaenDigitizerReadout>("CAEN", true),
      m_initialized(false),
      m_logger("caen_reader::Crate") {}

Crate::~Crate() {

   // Force a finalization, as that often needs to close devices and
   // free up memory:
   if (m_initialized) {
      m_logger << msg::WARNING
               << tr("Object getting deleted without being finalized!")
               << msg::endmsg;
      finalize();
   }
}

StatusCode Crate::initialize() {

   // Initialize all the devices:
   DeviceMap_t::const_iterator itr = m_devices.begin();
   DeviceMap_t::const_iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      CHECK(itr->second->initialize());
   }

   // Remember the object's internal state:
   m_initialized = true;

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

StatusCode Crate::finalize() {

   // Don't do anything if the object is not initialized:
   if (!m_initialized) {
      m_logger << msg::WARNING
               << tr("Object not in initialized state, not finalizing")
               << msg::endmsg;
      return StatusCode::SUCCESS;
   }

   // Finalize all the devices:
   DeviceMap_t::const_iterator itr = m_devices.begin();
   DeviceMap_t::const_iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      CHECK(itr->second->finalize());
   }

   // Remember the object's internal state:
   m_initialized = false;

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

StatusCode Crate::start() {

   // Start all the devices:
   DeviceMap_t::const_iterator itr = m_devices.begin();
   DeviceMap_t::const_iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      CHECK(itr->second->start());
   }

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

StatusCode Crate::stop() {

   // Stop all the devices:
   DeviceMap_t::const_iterator itr = m_devices.begin();
   DeviceMap_t::const_iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      CHECK(itr->second->stop());
   }

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

/**
 * Even though in the current setup we will always only have
 * one device in this "crate", in order to be compatible with the
 * other parts of CDA, one has to package the data coming from this
 * one device into a full ev::Event object.
 *
 * @returns A full even read from the device(s)
 */
ev::Event Crate::readEvent() const {

   // The event object to return:
   ev::Event event;

   // Read out all the devices:
   DeviceMap_t::const_iterator itr = m_devices.begin();
   DeviceMap_t::const_iterator end = m_devices.end();
   for (; itr != end; ++itr) {
      // Read out the event from a single constituent:
      event.addFragment(itr->second->readEvent());
   }

   // Return the event:
   return event;
}

}  // namespace caen_reader
