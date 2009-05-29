// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_DEVICES_T4300B_READOUT_H
#define CDA_CORE_DEVICES_T4300B_READOUT_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Readout.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Readout.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace t4300b {

   /**
    *  @short T4300B CAMAC readout
    *
    *         This class can handle a CAMAC device of this type,
    *         initialise it at the beginning of data taking and read
    *         out event fragments from it when a LAM signal arrived at
    *         the application.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Readout : virtual public dev::Readout,
                   virtual public Device {

   public:
      /// Constructor
      Readout();

      /// Initialise the CAMAC device for data acquisition
      virtual bool initialize( camac::Crate& crate ) const;
      /// Read the contents of the device
      virtual ev::Fragment readEvent( camac::Crate& crate ) const;
      /// Clear the module to receive a new event
      virtual bool clear( camac::Crate& crate ) const;

   private:
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Readout

} // namespace t4300b

#endif // CDA_CORE_DEVICES_T4300B_READOUT_H
