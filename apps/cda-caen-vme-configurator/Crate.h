// Dear emacs, this is -*- c++ -*-
#ifndef CDA_APPS_CDA_CAEN_VME_CONFIGURATOR_CRATE_H
#define CDA_APPS_CDA_CAEN_VME_CONFIGURATOR_CRATE_H

// CDA include(s).
#include "caen/VmeBus.h"
#include "common/StatusCode.h"
#include "device/Crate.h"
#include "device/ICaenVmeConfigurable.h"
#include "msg/Logger.h"

// Qt include(s).
#include <QtCore/QCoreApplication>

/// Namespace for the CAEN VME configurator class(es)
///
/// This namespace is only here for being fancy. There's no danger of the
/// classes used in the cda-caen-vme-configurator application to clash with
/// classes defined elsewhere...
///
/// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
///
namespace caen_vme_configurator {

/// Class representing a crate of CAEN VME devices to be configured
///
/// This class describes all the devices configured within the VME crate
/// that are in some way used during the data taking. But which do not
/// actually provide readout data for the DAQ system.
///
/// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
///
class Crate : public dev::Crate<dev::ICaenVmeConfigurable> {

   // To get the tr() function.
   Q_DECLARE_TR_FUNCTIONS(caen_vme_configurator::Crate)

public:
   /// Default constructor
   Crate();

   /// Function configuring all of the devices in the crate
   StatusCode configure();

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

   /// Message logger object
   mutable msg::Logger m_logger;

};  // class Crate

}  // namespace caen_vme_configurator

#endif  // CDA_APPS_CDA_CAEN_VME_CONFIGURATOR_CRATE_H
