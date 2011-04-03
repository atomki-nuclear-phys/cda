// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_DISK_H
#define CDA_DEVICES_AD413A_DISK_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>

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

namespace ad413a {

   /**
    *  @short Class capable of writing HBOOK files with info coming from an AD413A device
    *
    *         This class is used to write information coming from an AD413A device
    *         to disk.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Disk : public virtual dev::Disk,
                public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ad413a::Disk )

   public:
      /// Constructor
      Disk();

      /// Function initializing the device
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      /// LUT for the ntuple array indices
      int m_ntupleTable[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Disk

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_DISK_H
