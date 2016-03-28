// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T4300B_CERNLIBDISK_H
#define CDA_DEVICES_T4300B_CERNLIBDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICernlibDisk.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace t4300b {

   /**
    *  @short T4300B CERNLIB ntuple producer
    *
    *         This class can be used to write the data read out from a
    *         T4300B CAMAC QCD into a CERNLIB ntuple for further analysis.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibDisk : public virtual dev::ICernlibDisk,
                       public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( t4300b::CernlibDisk )

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

} // namespace t4300b

#endif // CDA_DEVICES_T4300B_CERNLIBDISK_H
