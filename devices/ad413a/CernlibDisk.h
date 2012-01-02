// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_CERNLIBDISK_H
#define CDA_DEVICES_AD413A_CERNLIBDISK_H

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

namespace ad413a {

   /**
    *  @short Class capable of writing HBOOK files with info coming from the device
    *
    *         This class is used to write information coming from an AD413A device
    *         to disk.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibDisk : public virtual dev::CernlibDisk,
                       public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ad413a::CernlibDisk )

   public:
      /// Constructor
      CernlibDisk();

      /// Function initializing the device
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      /// LUT for the ntuple array indices
      int m_ntupleTable[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class CernlibDisk

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_CERNLIBDISK_H
