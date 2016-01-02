// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T4300B_ROOTDISK_H
#define CDA_DEVICES_T4300B_ROOTDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/IRootDisk.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/IRootDisk.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace t4300b {

   /**
    *  @short ROOT file writer for a T4300B CAMAC device
    *
    *         This class can write out the charges converted by
    *         a T4300B CAMAC device into a ROOT file.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class RootDisk : public virtual dev::IRootDisk,
                    public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( t4300b::RootDisk )

   public:
      /// Constructor
      RootDisk();

      /// Function initializing the device
      virtual bool initialize( root::NTupleMgr& nmgr );

      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment ) const;

   private:
      /// The values read from the device
      mutable unsigned int m_values[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class RootDisk

} // namespace t4300b

#endif // CDA_DEVICES_T4300B_ROOTDISK_H
