// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAEN_READER_CRATE_H
#define CDA_APPS_CDA_CAEN_READER_CRATE_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CaenReadout.h"
#   include "cdacore/caen/Crate.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CaenReadout.h"
#   include "caen/Crate.h"
#   include "event/Event.h"
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
namespace caen {
   class Digitizer;
}

/**
 *  @short Namespace for the CAEN readout class(es)
 *
 *         This namespace is only here for being fancy. There's no
 *         danger of the classes used in the cda-caen-readout application
 *         to clash with classes defined elsewhere...
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace caen_reader {

   /**
    *  @short Class representing a "crate" of CAEN devices for readout
    *
    *         In keeping with the design of the rest of the code, the
    *         CAEN readout device(s) are handled through a "crate"
    *         object. Even though there is no physical crate involved
    *         in this case.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Crate : public caen::Crate< dev::CaenReadout > {

   public:
      /// Default constructor
      Crate();

      /// Function initializing all CAEN devices
      bool initialize( caen::Digitizer& dgtz ) const;
      /// Function reading out one event from the crate
      ev::Event readEvent( caen::Digitizer& dgtz ) const;

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Crate

} // namespace caen_reader

#endif // CDA_APPS_CDA_CAEN_READER_CRATE_H