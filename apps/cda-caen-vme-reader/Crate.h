// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAEN_VME_READER_CRATE_H
#define CDA_APPS_CDA_CAEN_VME_READER_CRATE_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "caen/VmeBus.h"
#include "common/StatusCode.h"
#include "device/Crate.h"
#include "device/ICaenVmeReadout.h"
#include "event/Event.h"
#include "msg/Logger.h"

/**
 *  @short Namespace for the CAEN VME readout class(es)
 *
 *         This namespace is only here for being fancy. There's no
 *         danger of the classes used in the cda-caen-vme-readout application
 *         to clash with classes defined elsewhere...
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
namespace caen_vme_reader {

/**
 *  @short Class representing a crate of CAEN VME devices for readout
 *
 *         This class describes all the devices used in the data acquisition
 *         from inside a single VME crate.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class Crate : public dev::Crate<dev::ICaenVmeReadout> {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(caen_vme_reader::Crate)

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

   /// Check if the devices appear to be in sync with each other
   bool devicesAreInSync() const;

protected:
   /// Read the crate specific options from binary input
   virtual StatusCode readCrateConfig(QIODevice& dev);
   /// Read the crate specific options from XML input
   virtual StatusCode readCrateConfig(const QDomElement& node);

private:
   /// Common VME bus object used by all the devices
   caen::VmeBus m_vmeBus;

   /// Type of the VME crate controller
   caen::VmeBus::BoardType m_type;
   /// Link number for connecting to the VME crate controller
   short m_linkNumber;
   /// Board number for connecting to the VME crate controller
   short m_boardNumber;
   /// Flag for controlling the device synchronisation checks
   bool m_checkDeviceSync;

   bool m_initialized;            ///< "Initialized state" of the device
   mutable msg::Logger m_logger;  ///< Message logger object

};  // class Crate

}  // namespace caen_vme_reader

#endif  // CDA_APPS_CDA_CAEN_VME_READER_CRATE_H
