// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_ROOTDISK_H
#define CDA_DEVICES_AD1000_ROOTDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/RootDisk.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/RootDisk.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace ad1000 {

   /**
    *  @short ROOT file writer for an AD1000 CAMAC device
    *
    *         This class can write out the single value converted by the
    *         AD1000 device. Hence it's very simple.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class RootDisk : public virtual dev::RootDisk,
                    public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ad1000::RootDisk )

   public:
      /// Constructor
      RootDisk();

      /// Function initializing the device
      virtual bool initialize( root::NTupleMgr& nmgr );

      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment ) const;

   private:
      mutable unsigned int m_value; ///< The converted signal's value

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class RootDisk

} // namespace ad1000

#endif // CDA_DEVICES_AD1000_ROOTDISK_H
