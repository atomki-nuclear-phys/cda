// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD2249A_CERNLIBDISK_H
#define CDA_DEVICES_AD2249A_CERNLIBDISK_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CernlibDisk.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#else
#   include "device/CernlibDisk.h"
#   include "msg/Logger.h"
#   include "cernlib/NTupleMgr.h"
#endif

// Local include(s):
#include "Device.h"

namespace ad2249a {

   /**
    *  @short AD2249A CERNLIB ntuple writer
    *
    *         This class can be used to write the information collected
    *         from an AD2249A CAMAC ADC device to a CERNLIB ntuple.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CernlibDisk : public virtual dev::CernlibDisk,
                       public virtual Device {

   public:
      /// Constructor
      CernlibDisk();

      /// Function initializing the device
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      /// LUT for the ntuple variable positions
      int m_ntupleTable[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class CernlibDisk

} // namespace ad2249a

#endif // CDA_DEVICES_T2249A_CERNLIBDISK_H
