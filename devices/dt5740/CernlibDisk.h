// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_CERNLIBDISK_H
#define CDA_DEVICES_DT5740_CERNLIBDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CernlibDisk.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CernlibDisk.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace dt5740 {

   /**
    *  @short Data file writer for the DT5740
    *
    *         This class is used to write HBOOK ntuples for the
    *         DT5740 CAEN digitizer.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibDisk : public virtual dev::CernlibDisk,
                       public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( dt5740::CernlibDisk )

   public:
      /// Constructor
      CernlibDisk();

      /// Function initializing the device
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      mutable msg::Logger m_logger; ///< Message logger object

   }; // class CernlibDisk

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_CERNLIBDISK_H
