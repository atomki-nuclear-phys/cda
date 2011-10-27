// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_S9418T_READOUT_H
#define CDA_DEVICES_S9418T_READOUT_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/VmeReadout.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/VmeReadout.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace s9418t {

   /**
    *  @short Class reading out an S9418T module
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Readout : virtual public dev::VmeReadout,
                   virtual public Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( s9418t::Readout )

   public:
      /// Default constructor
      Readout();

      /// Function starting the readout of the device
      virtual bool start( vme::VmeBus& bus ) const;
      /// Function stopping the readout of the device
      virtual bool stop( vme::VmeBus& bus ) const;

      /// Function initializing the device
      virtual bool initialize( vme::VmeBus& bus ) const;
      /// Function reading the current data from the device
      virtual ev::Fragment readEvent( vme::VmeBus& bus ) const;

   private:
      static const short CONTROL_REGISTER_ADDR = 0x44;
      static const short OFFSET_REGISTER_ADDR = 0x40;
      static const short RANGE_REGISTER_ADDR = 0x42;
      static const short AUTOCALIB_REGISTER_ADDR = 0x46;
      static const short VSN_H_REGISTER_ADDR = 0x48;
      static const short VSN_L_REGISTER_ADDR = 0x4a;
      static const short STATUS_REGISTER_ADDR = 0x4c;

      static const short FIFO_ADDR = 0x100;

      static const int FIFO_EMPTY_VALUE = 0x7fffffff;
      static const int FIFO_FOOTER_MASK = 0x40000000;
      static const int FIFO_EEVENT_MASK = 0x80000000;
      static const int FIFO_DWORD_MASK  = 0x01f00000;
      static const int FIFO_DATA_MASK   = 0x00000fff;

      static const short START_ACQ_VALUE = 0;
      static const short STOP_ACQ_VALUE = 1;
      static const short COMMON_STOP_MASK = 0x2;
      static const short CALIB_DISABLED_VALUE = 255;

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class Readout

} // namespace s9418t

#endif // CDA_DEVICES_S9418T_READOUT_H
