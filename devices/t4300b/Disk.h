// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T4300B_DISK_H
#define CDA_DEVICES_T4300B_DISK_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Disk.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#else
#   include "device/Disk.h"
#   include "msg/Logger.h"
#   include "cernlib/NTupleMgr.h"
#endif

// Local include(s):
#include "Device.h"

namespace t4300b {

   class Disk : public virtual dev::Disk,
                public virtual Device {

   public:
      /// Constructor
      Disk();

      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      int m_ntupleTable[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Disk

} // namespace t4300b

#endif // CDA_DEVICES_T4300B_DISK_H
