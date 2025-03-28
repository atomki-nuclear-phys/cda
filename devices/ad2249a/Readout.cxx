// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#include "camac/Crate.h"
#include "event/Fragment.h"

// Local include(s):
#include "Readout.h"

namespace ad2249a {

Readout::Readout() : m_logger("ad2249a::Readout") {}

/**
 * The initialisation codes are hard-coded into this function.
 * It should be okay, as this is the only class that has to access
 * the device anyway.
 */
bool Readout::initialize(camac::Crate& crate) const {

   //
   // Clear the module:
   //
   crate.writeWord(m_slot, 0, 9, 0);

   if (m_generateLam) {
      m_logger << msg::DEBUG
               << tr("Initialising AD2249A ADC in slot %1 "
                     "to generate LAM")
                      .arg(m_slot)
               << msg::endmsg;
      crate.writeWord(m_slot, 0, 26, 0);
   } else {
      m_logger << msg::DEBUG
               << tr("Initialising AD2249A ADC in slot %1 "
                     "NOT to generate LAM")
                      .arg(m_slot)
               << msg::endmsg;
      crate.writeWord(m_slot, 0, 24, 0);
   }

   return true;
}

/**
 * The function creates a new event fragment and configures it with
 * this device's module number. Then it reads out all the configured
 * subaddresses of the device and adds one data word for each of them
 * to the event fragment. In the data fragment both the subaddress of
 * the channel producing the data, and the data itself are encoded
 * the following way:
 *
 *   - uppermost 8 bits: Subaddress number
 *   - lower 24 bits: Data
 *
 * Since there are only 12 subaddresses on the device and it's an
 * 10-bit ADC (as far as I remember), this should leave plenty of space
 * for both quantities...
 */
std::unique_ptr<ev::Fragment> Readout::readEvent(camac::Crate& crate) const {

   // Create a new event fragment:
   std::unique_ptr<ev::Fragment> fragment(new ev::Fragment());
   fragment->setModuleID(m_slot);

   // Read out all the configured channels:
   for (int i = 0; i < NUMBER_OF_SUBADDRESSES; ++i) {
      if (m_channels[i]) {
         const uint32_t channel = crate.readWord(m_slot, i, 0);
         const uint32_t dword = (i << 24) | (channel & 0xffffff);
         fragment->addDataWord(dword);
      }
   }

   return fragment;
}

bool Readout::clear(camac::Crate& crate) const {

   //
   // Clear the module:
   //
   crate.writeWord(m_slot, 0, 10, 0);

   return true;
}

}  // namespace ad2249a
