// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICES_T2228A_READOUT_H
#define CDA_CORE_DEVICES_T2228A_READOUT_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICamacReadout.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace t2228a {

/**
 *  @short T2228A CAMAC readout
 *
 *         This class can handle a CAMAC device of this type,
 *         initialise it at the beginning of data taking and read
 *         out event fragments from it when a LAM signal arrived at
 *         the application.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class Readout : virtual public dev::ICamacReadout, virtual public Device {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(t2228a::Readout)

public:
   /// Constructor
   Readout();

   /// Initialise the CAMAC device for data acquisition
   virtual bool initialize(camac::Crate& crate) const;
   /// Read the contents of the device
   virtual std::unique_ptr<ev::Fragment> readEvent(camac::Crate& crate) const;
   /// Clear the module to receive a new event
   virtual bool clear(camac::Crate& crate) const;

private:
   mutable msg::Logger m_logger;  ///< Message logging object

};  // class Readout

}  // namespace t2228a

#endif  // CDA_CORE_DEVICES_T2228A_READOUT_H
