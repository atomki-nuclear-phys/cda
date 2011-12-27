// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T2228A_CERNLIBDISK_H
#define CDA_DEVICES_T2228A_CERNLIBDISK_H

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

namespace t2228a {

   /**
    *  @short Data file writer for the T2228A
    *
    *         This class is used to write out the information collected
    *         using a T2228A CAMAC device, into an HBOOK file. A single
    *         time value is stored per subaddress per event.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
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
      /// Internal table associating variables with ntuple columns
      int m_ntupleTable[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Disk

} // namespace t2228a

#endif // CDA_DEVICES_T2228A_CERNLIBDISK_H
