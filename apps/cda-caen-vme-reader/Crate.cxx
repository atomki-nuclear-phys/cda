// $Id$

// Qt include(s):
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"
#include "msg/Sender.h"

// Local include(s):
#include "Crate.h"

namespace caen_vme_reader {

Crate::Crate()
    : dev::Crate<dev::ICaenVmeReadout>("CAENVME", true),
      m_vmeBus(),
      m_type(caen::VmeBus::BOARD_V1718),
      m_linkNumber(0),
      m_boardNumber(0),
      m_checkDeviceSync(true),
      m_initialized(false),
      m_logger("caen_vme_reader::Crate") {}

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

   // Open the connection to the VME bus:
   CHECK(m_vmeBus.open(m_type, m_linkNumber, m_boardNumber));

   // Initialize all the devices:
   for (auto& itr : m_devices) {
      CHECK(itr.second->initialize(m_vmeBus));
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
   for (auto& itr : m_devices) {
      CHECK(itr.second->finalize());
   }

   // Close the VME bus:
   CHECK(m_vmeBus.close());

   // Remember the object's internal state:
   m_initialized = false;

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

StatusCode Crate::start() {

   // Start all the devices:
   for (auto& itr : m_devices) {
      CHECK(itr.second->start());
   }

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

StatusCode Crate::stop() {

   // Stop all the devices:
   for (auto& itr : m_devices) {
      CHECK(itr.second->stop());
   }

   // Show that we were successful:
   return StatusCode::SUCCESS;
}

ev::Event Crate::readEvent() const {

   // The event object to return:
   ev::Event event;

   // Read out all the devices:
   for (auto& itr : m_devices) {
      event.addFragment(itr.second->readEvent());
   }

   // Return the event:
   return event;
}

bool Crate::devicesAreInSync() const {

   // If the synchronisation is not to be checked, return right away:
   if (!m_checkDeviceSync) {
      return true;
   }

   // A simple security check:
   if (!m_devices.size()) {
      // Not sure what's happening here, but sure...
      return true;
   }

   // Get the number of events processed by the first device:
   const size_t eventsProcessed = m_devices.begin()->second->eventsProcessed();

   // Lambda used in printing the number of events seen / converted by each
   // device.
   auto printEvents = [this](msg::Logger& logger, const DeviceMap_t& devices,
                             msg::Level level) {
      if (msg::Sender::instance()->getMinLevel() <= level) {
         for (const auto& itr : devices) {
            logger << level
                   << tr("Device %1 (%2) processed %3 events")
                          .arg(itr.first)
                          .arg(itr.second->deviceName())
                          .arg(itr.second->eventsProcessed())
                   << msg::endmsg;
         }
      }
   };

   // Check if every device reports the same:
   for (const auto& itr : m_devices) {
      if (eventsProcessed != itr.second->eventsProcessed()) {
         m_logger << msg::WARNING
                  << tr("Devices are not in sync with each other!")
                  << msg::endmsg;
         printEvents(m_logger, m_devices, msg::WARNING);
         return false;
      }
   }

   // If yes, then they are in sync:
   printEvents(m_logger, m_devices, msg::DEBUG);
   return true;
}

StatusCode Crate::readCrateConfig(QIODevice& dev) {

   // Create the object used for reading the data:
   QDataStream input(&dev);
   input.setVersion(QDataStream::Qt_4_0);

   // Read the connector type:
   quint16 type = 0;
   input >> type;
   m_type = static_cast<caen::VmeBus::BoardType>(type);

   // Read the link number:
   qint16 linkNumber = 0;
   input >> linkNumber;
   m_linkNumber = linkNumber;

   // Read the board number:
   qint16 boardNumber = 0;
   input >> boardNumber;
   m_boardNumber = boardNumber;

   // Read the device synchronisation check setting:
   quint16 checkDeviceSync = 1;
   input >> checkDeviceSync;
   m_checkDeviceSync = checkDeviceSync;

   // Tell the user what happened:
   REPORT_VERBOSE(tr("Read the following VME controller parameters:\n"
                     "  type          : %1\n"
                     "  link number   : %2\n"
                     "  board number  : %3\n"
                     "  check dev sync: %4")
                      .arg(m_type)
                      .arg(m_linkNumber)
                      .arg(m_boardNumber)
                      .arg(m_checkDeviceSync));

   // Return gracefully:
   return StatusCode::SUCCESS;
}

StatusCode Crate::readCrateConfig(const QDomElement& node) {

   // A helper variable:
   bool ok = true;

   // Read the controller type:
   const int type = node.attribute("ControllerType", "0").toInt(&ok);
   CHECK(ok);
   m_type = static_cast<caen::VmeBus::BoardType>(type);

   // Read the link number:
   const int linkNumber = node.attribute("LinkNumber", "0").toInt(&ok);
   CHECK(ok);
   m_linkNumber = static_cast<short>(linkNumber);

   // Read the board number:
   const int boardNumber = node.attribute("BoardNumber", "0").toInt(&ok);
   CHECK(ok);
   m_boardNumber = static_cast<short>(boardNumber);

   // Read the device synchronisation check setting:
   const int checkDeviceSync =
       node.attribute("CheckDeviceSync", "1").toInt(&ok);
   CHECK(ok);
   m_checkDeviceSync = static_cast<bool>(checkDeviceSync);

   // Return gracefully:
   return StatusCode::SUCCESS;
}

}  // namespace caen_vme_reader
