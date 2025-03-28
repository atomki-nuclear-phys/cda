// $Id$

// Qt include(s):
#include <QtCore/QDataStream>
#include <QtCore/QIODevice>
#include <QtXml/QDomElement>

// CDA include(s):
#include "common/errorcheck.h"

// Local include(s):
#include "Device.h"

namespace ad413a {

//
// Make sure that the following Qt classes are available in the
// current namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QIODevice);
using QT_PREPEND_NAMESPACE(QDataStream);
using QT_PREPEND_NAMESPACE(QDomElement);

Device::Device()
    : m_slot(-1), m_generateLam(false), m_gate(0), m_logger("ad413a::Device") {}

StatusCode Device::readConfig(QIODevice& dev) {

   REPORT_VERBOSE(tr("Reading configuration from binary input"));

   clear();

   QDataStream input(&dev);
   input.setVersion(QDataStream::Qt_4_0);
   input >> m_slot;
   input >> m_generateLam;
   input >> m_gate;
   quint32 number_of_channels;
   input >> number_of_channels;

   REPORT_VERBOSE(tr(" - Slot        : %1\n"
                     " - GenerateLAM : %2\n"
                     " - Gate        : %3\n"
                     " - Subaddresses: %4")
                      .arg(m_slot)
                      .arg(m_generateLam)
                      .arg(m_gate)
                      .arg(number_of_channels));

   for (quint32 i = 0; i < number_of_channels; ++i) {
      std::unique_ptr<ChannelConfig> channel(new ChannelConfig());
      CHECK(channel->readConfig(dev));
      if ((channel->getSubaddress() >= 0) &&
          (channel->getSubaddress() < NUMBER_OF_SUBADDRESSES)) {
         if (m_channels[channel->getSubaddress()]) {
            m_logger << msg::WARNING
                     << tr("Redefining channel number: %1")
                            .arg(channel->getSubaddress())
                     << msg::endmsg;
         }
         m_channels[channel->getSubaddress()].swap(channel);
      } else {
         REPORT_ERROR(
             tr("There was a problem reading the configuration "
                "of one channel"));
         return StatusCode::FAILURE;
      }
   }

   return StatusCode::SUCCESS;
}

StatusCode Device::writeConfig(QIODevice& dev) const {

   REPORT_VERBOSE(tr("Writing configuration to binary output"));

   QDataStream output(&dev);
   output.setVersion(QDataStream::Qt_4_0);
   output << m_slot;
   output << m_generateLam;
   output << m_gate;

   // Count the number of configured channels:
   quint32 number_of_channels = 0;
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      if (m_channels[i])
         ++number_of_channels;
   }

   // Write the number of channels to follow:
   output << number_of_channels;

   // Write the channel configurations:
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      if (m_channels[i]) {
         CHECK(m_channels[i]->writeConfig(dev));
      }
   }

   return StatusCode::SUCCESS;
}

StatusCode Device::readConfig(const QDomElement& element) {

   REPORT_VERBOSE(tr("Reading configuration from XML input"));

   clear();

   bool ok;

   m_slot = element.attribute("Slot", "0").toInt(&ok);
   CHECK(ok);

   m_generateLam = element.attribute("GenerateLAM", "0").toShort(&ok);
   CHECK(ok);

   m_gate = element.attribute("Gate", "0").toShort(&ok);
   CHECK(ok);

   REPORT_VERBOSE(tr(" - Slot       : %1\n"
                     " - GenerateLAM: %2\n"
                     " - Gate       : %3")
                      .arg(m_slot)
                      .arg(m_generateLam)
                      .arg(m_gate));

   for (int i = 0; i < element.childNodes().size(); ++i) {

      // Only process "Channel" type child-nodes:
      if (element.childNodes().at(i).nodeName() != "Channel") {
         continue;
      }

      std::unique_ptr<ChannelConfig> channel(new ChannelConfig());
      const QDomElement& chElement = element.childNodes().at(i).toElement();
      CHECK(channel->readConfig(chElement));

      if ((channel->getSubaddress() >= 0) &&
          (channel->getSubaddress() < NUMBER_OF_SUBADDRESSES)) {
         if (m_channels[channel->getSubaddress()]) {
            m_logger << msg::WARNING
                     << tr("Redefining channel number: %1")
                            .arg(channel->getSubaddress())
                     << msg::endmsg;
         }
         m_channels[channel->getSubaddress()].swap(channel);
      } else {
         REPORT_ERROR(
             tr("There was a problem reading the configuration "
                "of one channel"));
         return StatusCode::FAILURE;
      }
   }

   return StatusCode::SUCCESS;
}

StatusCode Device::writeConfig(QDomElement& element) const {

   REPORT_VERBOSE(tr("Writing configuration to XML output"));

   element.setAttribute("Slot", m_slot);
   element.setAttribute("GenerateLAM", m_generateLam);
   element.setAttribute("Gate", m_gate);

   //
   // Create a new node for the configuration of each channel:
   //
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      if (m_channels[i]) {
         QDomElement ch_element =
             element.ownerDocument().createElement("Channel");
         CHECK(m_channels[i]->writeConfig(ch_element));
         element.appendChild(ch_element);
      }
   }

   return StatusCode::SUCCESS;
}

const QString& Device::deviceName() const {

   static const QString name("AD413A");
   return name;
}

unsigned int Device::getID() const {

   return m_slot;
}

void Device::setID(unsigned int value) {

   m_slot = value;
   return;
}

void Device::clear() {

   m_slot = 0;
   m_generateLam = false;
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      m_channels[i].reset();
   }

   return;
}

}  // namespace ad413a
