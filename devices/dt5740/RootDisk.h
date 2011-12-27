// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_DT5740_ROOTDISK_H
#define CDA_DEVICES_DT5740_ROOTDISK_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/RootDisk.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/RootDisk.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"
#include "Processor.h"

namespace dt5740 {

   /**
    *  @short ROOT file writer for a DT5740 device
    *
    *         This class can write out all the data collected by a DT5740
    *         device into a ROOT file. The class can either just write out
    *         the reconstructed parameters, or the raw data collected from
    *         the channels as well. (Controllable using a property found
    *         in dt5740::Device.)
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class RootDisk : public virtual dev::RootDisk,
                    public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( dt5740::RootDisk )

   public:
      /// Constructor
      RootDisk();

      /// Function initializing the device
      virtual bool initialize( root::NTupleMgr& nmgr );

      /// Function filling the ntuple
      virtual bool writeEvent( const ev::Fragment& fragment ) const;

   private:
      mutable Processor m_processor; ///< Object reconstructing the signals
      mutable caen::Digitizer::EventInfo m_eventInfo; ///< Decoded event info
      mutable caen::Digitizer::EventData16Bit m_eventData; ///< Decoded event data

      /// The raw data collected from the different channels
      mutable std::vector< uint16_t >
      m_channelData[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// The reconstructed time values
      mutable double
      m_times[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];
      /// The reconstructed energy values
      mutable double
      m_energies[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ];

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class RootDisk

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_ROOTDISK_H
