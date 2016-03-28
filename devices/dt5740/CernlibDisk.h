// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_CERNLIBDISK_H
#define CDA_DEVICES_DT5740_CERNLIBDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICernlibDisk.h"
#include "caen/Digitizer.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"
#include "Processor.h"

namespace dt5740 {

   /**
    *  @short Data file writer for the DT5740
    *
    *         This class is used to write HBOOK ntuples for the
    *         DT5740 CAEN digitizer.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibDisk : public virtual dev::ICernlibDisk,
                       public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( dt5740::CernlibDisk )

   public:
      /// Constructor
      CernlibDisk();

      /// Function initializing the device
      virtual bool initialize( cernlib::NTupleMgr& nmgr );
      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment,
                               cernlib::NTupleMgr& nmgr ) const;

   private:
      mutable Processor m_processor; ///< Object reconstructing the signals
      /// "Map" for the created variables
      int m_ntupleMap[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ][ 2 ];
      mutable caen::Digitizer::EventInfo m_eventInfo; ///< Decoded event info
      mutable caen::Digitizer::EventData16Bit m_eventData; ///< Decoded event data

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class CernlibDisk

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_CERNLIBDISK_H
