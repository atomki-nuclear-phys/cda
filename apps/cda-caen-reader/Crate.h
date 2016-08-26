// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAEN_READER_CRATE_H
#define CDA_APPS_CDA_CAEN_READER_CRATE_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICaenDigitizerReadout.h"
#include "device/Crate.h"
#include "event/Event.h"
#include "msg/Logger.h"
#include "common/StatusCode.h"

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
   class Crate : public dev::Crate< dev::ICaenDigitizerReadout > {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( caen_reader::Crate )

   public:
      /// Default constructor
      Crate();
      /// Destructor
      ~Crate();

      /// Function initializing all CAEN devices
      StatusCode initialize();
      /// Function finalizing all CAEN devices
      StatusCode finalize();

      /// Start the acquisition on all the devices
      StatusCode start();
      /// Stop the acquisition on all the devices
      StatusCode stop();

      /// Function reading out one event from the crate
      ev::Event readEvent() const;

   private:
      bool m_initialized; ///< "Initialized state" of the device
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Crate

} // namespace caen_reader

#endif // CDA_APPS_CDA_CAEN_READER_CRATE_H
