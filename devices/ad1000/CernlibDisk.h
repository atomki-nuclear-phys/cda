// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_CERNLIBDISK_H
#define CDA_DEVICES_AD1000_CERNLIBDISK_H

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

namespace ad1000 {

   /**
    *  @short AD1000 CERNLIB ntuple producer
    *
    *         This class can be used to write the data read out of an AD1000
    *         ADC into a CERNLIB ntuple for further analysis.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibDisk : public virtual dev::CernlibDisk,
                       public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ad1000::CernlibDisk )

   public:
      /// Constructor
      CernlibDisk();

      /// Initialize the ntuple
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Fill the ntuple with one event
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      /// Location of the data within the ntuple array
      int m_ntupleNumber;

      mutable msg::Logger m_logger; ///< Message logging object

   }; // class CernlibDisk

} // namespace ad1000

#endif // CDA_DEVICES_AD1000_CERNLIBDISK_H
