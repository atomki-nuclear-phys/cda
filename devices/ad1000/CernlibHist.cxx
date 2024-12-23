// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#include "cernlib/HistMgr.h"
#include "event/Fragment.h"

// Local include(s):
#include "CernlibHist.h"

namespace ad1000 {

CernlibHist::CernlibHist() : m_histNumber(0), m_logger("ad1000::CernlibHist") {}

bool CernlibHist::initialize(cernlib::HistMgr& hmgr) {

   m_logger << msg::DEBUG << tr("Initialising histogram(s)") << msg::endmsg;

   // Book the histogram and remember its ID:
   m_histNumber =
       hmgr.book_1d(tr("%1 (%2 slot %3)")
                        .arg(m_channel.getName())
                        .arg(deviceName())
                        .arg(getID())
                        .toLatin1()
                        .constData(),
                    m_channel.getNumberOfChannels(), m_channel.getLowerBound(),
                    m_channel.getUpperBound());

   REPORT_VERBOSE(tr("Created histogram %1 with name: \"%2\"")
                      .arg(m_histNumber)
                      .arg(m_channel.getName()));

   return true;
}

bool CernlibHist::displayEvent(const ev::Fragment& fragment,
                               const cernlib::HistMgr& hmgr) const {

   // Access the data words:
   const ev::Fragment::Payload_t& dataWords = fragment.getDataWords();

   // Sanity check:
   if (dataWords.size() != 1) {
      REPORT_ERROR(tr("Received %1 data words. "
                      "Was supposed to receive 1.")
                       .arg(dataWords.size()));
      return false;
   }

   // Decode the data word:
   const unsigned int data = (dataWords.front() & 0xffffff);

   // Fill the histogram:
   hmgr.fill_1d(m_histNumber, (double)data, 1.0);

   return true;
}

}  // namespace ad1000
