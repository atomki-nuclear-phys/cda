
// Local include(s).
#include "Crate.h"

// CDA include(s).
#include "common/errorcheck.h"

// Qt include(s).
#include <QtCore/QDataStream>
#include <QtXml/QDomElement>

namespace caen_vme_configurator {

Crate::Crate()
    : dev::Crate<dev::ICaenVmeConfigurable>("CAENVME", true),
      m_vmeBus(),
      m_type(caen::VmeBus::BOARD_V1718),
      m_linkNumber(0),
      m_boardNumber(0),
      m_logger("caen_vme_configurator::Crate") {}

StatusCode Crate::configure() {

   // Open the connection to the VME bus.
   CHECK(m_vmeBus.open(m_type, m_linkNumber, m_boardNumber));

   // Configure all the devices.
   for (auto& itr : m_devices) {
      CHECK(itr.second->configure(m_vmeBus));
   }

   // Close the VME bus.
   CHECK(m_vmeBus.close());

   // Return gracefully.
   return StatusCode::SUCCESS;
}

StatusCode Crate::readCrateConfig(QIODevice& dev) {

   // Create the object used for reading the data.
   QDataStream input(&dev);
   input.setVersion(QDataStream::Qt_4_0);

   // Read the connector type.
   quint16 type = 0;
   input >> type;
   m_type = static_cast<caen::VmeBus::BoardType>(type);

   // Read the link number.
   qint16 linkNumber = 0;
   input >> linkNumber;
   m_linkNumber = linkNumber;

   // Read the board number.
   qint16 boardNumber = 0;
   input >> boardNumber;
   m_boardNumber = boardNumber;

   // Tell the user what happened.
   REPORT_VERBOSE(tr("Read the following VME controller parameters:\n"
                     "  type          : %1\n"
                     "  link number   : %2\n"
                     "  board number  : %3")
                      .arg(m_type)
                      .arg(m_linkNumber)
                      .arg(m_boardNumber));

   // Return gracefully.
   return StatusCode::SUCCESS;
}

StatusCode Crate::readCrateConfig(const QDomElement& node) {

   // A helper variable.
   bool ok = true;

   // Read the controller type.
   const int type = node.attribute("ControllerType", "0").toInt(&ok);
   CHECK(ok);
   m_type = static_cast<caen::VmeBus::BoardType>(type);

   // Read the link number.
   const int linkNumber = node.attribute("LinkNumber", "0").toInt(&ok);
   CHECK(ok);
   m_linkNumber = static_cast<short>(linkNumber);

   // Read the board number.
   const int boardNumber = node.attribute("BoardNumber", "0").toInt(&ok);
   CHECK(ok);
   m_boardNumber = static_cast<short>(boardNumber);

   // Return gracefully.
   return StatusCode::SUCCESS;
}

}  // namespace caen_vme_configurator
