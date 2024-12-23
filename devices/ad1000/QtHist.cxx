// $Id$

// Qt include(s):
#include <QStackedLayout>

// CDA include(s):
#include "common/errorcheck.h"
#include "event/Fragment.h"
#include "moni/Histogram.h"

// Local include(s):
#include "QtHist.h"

namespace ad1000 {

QtHist::QtHist(QWidget* parent, Qt::WindowFlags flags)
    : dev::QtHist(parent, flags), m_logger("dt5740::QtHist") {

   // Declare a minimum size for this widget:
   setMinimumSize(220, 200);

   // Create the "layout" for the histogram:
   m_layout.reset(new QStackedLayout(this));

   // Create the only histogram:
   m_hist.reset(new moni::Histogram());

   // Add the histogram to the layout:
   m_layout->addWidget(m_hist.get());
}

StatusCode QtHist::readConfig(QIODevice& dev) {

   // Read in the configuration using the base class:
   CHECK(Device::readConfig(dev));

   // Update the look of the widget:
   CHECK(initialize());

   // Return gracefully:
   return StatusCode::SUCCESS;
}

StatusCode QtHist::readConfig(const QDomElement& node) {

   // Read in the configuration using the base class:
   CHECK(Device::readConfig(node));

   // Update the look of the widget:
   CHECK(initialize());

   // Return gracefully:
   return StatusCode::SUCCESS;
}

bool QtHist::displayEvent(const ev::Fragment& fragment) {

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
   m_hist->fill((double)data, 1.0);

   return true;
}

bool QtHist::initialize() {

   // Set the properties of the histogram:
   m_hist->setTitle(m_channel.getName());
   m_hist->setNBins(m_channel.getNumberOfChannels());
   if (m_channel.getUpperBound() > m_hist->getLowerBound()) {
      m_hist->setUpperBound(m_channel.getUpperBound());
      m_hist->setLowerBound(m_channel.getLowerBound());
   } else {
      m_hist->setLowerBound(m_channel.getLowerBound());
      m_hist->setUpperBound(m_channel.getUpperBound());
   }

   return true;
}

}  // namespace ad1000
