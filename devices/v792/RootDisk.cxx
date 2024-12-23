// $Id$

// CDA include(s):
#include "common/errorcheck.h"
#include "event/Fragment.h"
#include "root/NTupleMgr.h"

// Local include(s):
#include "RootDisk.h"

namespace v792 {

RootDisk::RootDisk() : Device(), m_logger("v792::RootDisk") {}

bool RootDisk::initialize(root::NTupleMgr& nmgr) {

   // Loop over all configured channels:
   for (size_t i = 0; i < NUMBER_OF_CHANNELS; ++i) {
      if (m_channels[i]) {
         CHECK(nmgr.addVar(m_data[i], m_channels[i]->getName()));
         CHECK(nmgr.addVar(m_underThreshold[i],
                           m_channels[i]->getName() + "_un"));
         CHECK(nmgr.addVar(m_overflow[i], m_channels[i]->getName() + "_ov"));
      }
   }

   // Return gracefully:
   return true;
}

bool RootDisk::writeEvent(const ev::Fragment& fragment) const {

   // Reset all the ntuple variables:
   for (size_t i = 0; i < NUMBER_OF_CHANNELS; ++i) {
      m_data[i] = 0;
      m_underThreshold[i] = 0;
      m_overflow[i] = 0;
   }

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
      m_data[channel] = cd.data();
      m_underThreshold[channel] = cd.underThreshold();
      m_overflow[channel] = cd.overflow();
   }

   // Return gracefully:
   return true;
}

}  // namespace v792
