// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_V862_ROOTDISK_H
#define CDA_DEVICES_V862_ROOTDISK_H

// System include(s):
#include <array>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/IRootDisk.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace v862 {

   /// Class writing the data read from a V862 device to a ROOT file
   ///
   /// This class is used to write ROOT TTree-s with the data read out from
   /// a V862 device.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date$
   ///
   class RootDisk : public virtual dev::IRootDisk,
         public virtual Device {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v862::RootDisk )

   public:
      /// Default constructor
      RootDisk();

      /// Function initializing the device
      virtual bool initialize( root::NTupleMgr& nmgr );

      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment ) const;

   private:
      /// Helper variable for writing the read data
      mutable std::array< unsigned int, NUMBER_OF_CHANNELS > m_data;
      /// Helper variable for writing the under threshold bits
      mutable std::array< char, NUMBER_OF_CHANNELS > m_underThreshold;
      /// Helper variable for writing the overflow bits
      mutable std::array< char, NUMBER_OF_CHANNELS > m_overflow;

      /// Message logging object
      mutable msg::Logger m_logger;

   }; // class RootDisk

} // namespace v862

#endif // CDA_DEVICES_V862_ROOTDISK_H
