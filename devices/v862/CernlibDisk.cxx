// $Id$

// CDA include(s):
#include "cernlib/NTupleMgr.h"
#include "common/errorcheck.h"
#include "event/Fragment.h"

// Local include(s):
#include "CernlibDisk.h"

namespace v862 {

CernlibDisk::CernlibDisk()
    : Device(),
      m_dataTable(),
      m_unTable(),
      m_ovTable(),
      m_logger("v862::CernlibDisk") {}

bool CernlibDisk::initialize(cernlib::NTupleMgr& nmgr) {

   // Tell the user what's happening:
   m_logger << msg::DEBUG << tr("Initialising ntuple output") << msg::endmsg;

   // Loop over all configured subaddresses:
   for (size_t i = 0; i < NUMBER_OF_CHANNELS; ++i) {
      if (m_channels[i]) {
         // Book all variables for this channel:
         m_dataTable[i] = nmgr.addVar(m_channels[i]->getName());
         m_ovTable[i] = nmgr.addVar(m_channels[i]->getName() + "_ov");
         m_unTable[i] = nmgr.addVar(m_channels[i]->getName() + "_un");
      }
   }

   // Return gracefully:
   return true;
}

bool CernlibDisk::writeEvent(const ev::Fragment& fragment,
                             cernlib::NTupleMgr& nmgr) const {

   // Process the received fragment:
   for (auto data : fragment.getDataWords()) {

      // Decode the word:
      ChannelData cd(data);

      // Make sure that this is a valid channel:
      const size_t channel = cd.channel();
      if (channel >= NUMBER_OF_CHANNELS) {
         REPORT_ERROR(tr("Unexpected channel number found: %1").arg(channel));
         return false;
      }

      // Set the ntuple variables:
      CHECK(nmgr.setVar(m_dataTable[channel], cd.data()));
      CHECK(nmgr.setVar(m_unTable[channel], cd.underThreshold()));
      CHECK(nmgr.setVar(m_ovTable[channel], cd.overflow()));
   }

   // Return gracefully:
   return true;
}

}  // namespace v862
