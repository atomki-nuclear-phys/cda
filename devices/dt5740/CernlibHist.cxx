// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#include "cernlib/HistMgr.h"
#include "common/errorcheck.h"

// Local include(s):
#include "CernlibHist.h"

namespace dt5740 {

CernlibHist::CernlibHist() : m_processor(), m_logger("dt5740::CernlibHist") {

   // Reset the histogram map:
   for (int group = 0; group < NUMBER_OF_GROUPS; ++group) {
      for (int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
           ++channel) {
         for (int i = 0; i < 2; ++i) {
            m_histMap[group][channel][i] = -1;
         }
      }
   }
}

bool CernlibHist::initialize(cernlib::HistMgr& hmgr) {

   // Configure the signal processor:
   m_processor.setFraction(m_cfdFraction);
   m_processor.setDelay(m_cfdDelay);
   m_processor.setLength(m_cfdLength);
   m_processor.setSmoothWidth(m_gaussSmoothWidth);

   //
   // Book histograms for all the active channels:
   //
   for (int group = 0; group < NUMBER_OF_GROUPS; ++group) {
      for (int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
           ++channel) {

         // Check if the channel is active:
         const ChannelConfig* ch = m_groups[group].getChannel(channel);
         if (!ch)
            continue;

         // Book the time histogram:
         m_histMap[group][channel][0] = hmgr.book_1d(
             tr("%1 (%2 with device ID %3, channel %4)")
                 .arg(ch->getTimeName())
                 .arg(deviceName())
                 .arg(getID())
                 .arg(group * GroupConfig::CHANNELS_IN_GROUP + channel)
                 .toLatin1()
                 .constData(),
             ch->getTimeNumberOfChannels(), ch->getTimeLowerBound(),
             ch->getTimeUpperBound());

         // Book the energy histogram:
         m_histMap[group][channel][1] = hmgr.book_1d(
             tr("%1 (%2 with device ID %3, channel %4)")
                 .arg(ch->getEnergyName())
                 .arg(deviceName())
                 .arg(getID())
                 .arg(group * GroupConfig::CHANNELS_IN_GROUP + channel)
                 .toLatin1()
                 .constData(),
             ch->getEnergyNumberOfChannels(), ch->getEnergyLowerBound(),
             ch->getEnergyUpperBound());

         REPORT_VERBOSE(tr("Created time and energy monitoring histograms for "
                           "channel with raw name %1")
                            .arg(ch->getRawName()));
      }
   }

   return true;
}

bool CernlibHist::displayEvent(const ev::Fragment& fragment,
                               const cernlib::HistMgr& hmgr) const {

   // Decode the event fragment:
   CHECK(decode(fragment, m_eventInfo, m_eventData));

   // The reconstructed object to be used:
   Processor::Result res;

   //
   // Reconstruct the data in all channels, and fill the histograms:
   //
   for (int group = 0; group < NUMBER_OF_GROUPS; ++group) {
      for (int channel = 0; channel < GroupConfig::CHANNELS_IN_GROUP;
           ++channel) {

         // Check if the channel is active:
         const ChannelConfig* ch = m_groups[group].getChannel(channel);
         if (!ch)
            continue;

         // Reconstruct the data of this channel:
         CHECK(m_processor.reconstruct(
             m_eventData
                 .chData[group * GroupConfig::CHANNELS_IN_GROUP + channel],
             res));

         // Fill the histograms with the reconstructed data:
         hmgr.fill_1d(m_histMap[group][channel][0], res.time, 1.0);
         hmgr.fill_1d(m_histMap[group][channel][1], res.energy, 1.0);
      }
   }

   return true;
}

}  // namespace dt5740
