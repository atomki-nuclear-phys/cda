// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T2228A_CERNLIBDISK_H
#define CDA_DEVICES_T2228A_CERNLIBDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICernlibDisk.h"
#include "msg/Logger.h"

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
   class CernlibDisk : public virtual dev::ICernlibDisk,
                       public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( t2228a::CernlibDisk )

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

   }; // class CernlibDisk

} // namespace t2228a

#endif // CDA_DEVICES_T2228A_CERNLIBDISK_H
