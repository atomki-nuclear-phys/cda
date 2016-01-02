// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_ROOTDISK_H
#define CDA_DEVICES_AD413A_ROOTDISK_H

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

namespace ad413a {

   /**
    *  @short ROOT file writer for an AD413A CAMAC device
    *
    *         This class can write out the pulse heights converted by
    *         an AD413A CAMAC device into a ROOT file.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class RootDisk : public virtual dev::IRootDisk,
                    public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ad413a::RootDisk )

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

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_ROOTDISK_H
