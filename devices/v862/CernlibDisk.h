// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V862_CERNLIBDISK_H
#define CDA_DEVICES_V862_CERNLIBDISK_H

// System include(s):
#include <array>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICernlibDisk.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace v862 {

   /// Class writing the data read from a V862 to a HBOOK file
   ///
   /// This class is used by the cda-hbook-writer application to write CERNLIB
   /// hbook files with the data collected from a V862 device.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date$
   ///
   class CernlibDisk : public virtual dev::ICernlibDisk,
         public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v862::CernlibDisk )

   public:
      /// Constructor
      CernlibDisk();

      /// @name Function(s) inherited from dev::ICernlibDisk
      /// @{

      /// Function initializing the device
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

      /// @}

   private:
      /// Internal table associating data variables with ntuple columns
      std::array< int, NUMBER_OF_CHANNELS > m_dataTable;
      /// Internal table associating under threshold variables with ntuple columns
      std::array< int, NUMBER_OF_CHANNELS > m_unTable;
      /// Internal table associating overflow variables with ntuple columns
      std::array< int, NUMBER_OF_CHANNELS > m_ovTable;

      /// Message logging object
      mutable msg::Logger m_logger;

   }; // class CernlibDisk

} // namespace v862

#endif // CDA_DEVICES_V862_CERNLIBDISK_H
