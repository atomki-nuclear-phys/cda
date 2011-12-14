// $Id$

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/event/Fragment.h"
#else
#   include "event/Fragment.h"
#endif

// Local include(s):
#include "Readout.h"

namespace dt5740 {

   Readout::Readout()
      : dev::CaenReadout(), Device(), m_digitizer(),
        m_logger( "dt5740::Readout" ) {

   }

   Readout::~Readout() {

      finalize();
   }

   bool Readout::initialize() {

      return true;
   }

   bool Readout::finalize() {

      return true;
   }

   bool Readout::start() {

      return true;
   }

   bool Readout::stop() {

      return true;
   }

   ev::Fragment* Readout::readEvent() const {

      ev::Fragment* result = new ev::Fragment();

      return result;
   }

} // namespace dt5740
