// Dear emacs, this is -*- c++ -*-
// $Id%
#ifndef CDA_DEVICES_DT5740_CERNLIBHIST_H
#define CDA_DEVICES_DT5740_CERNLIBHIST_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CernlibHist.h"
#   include "cdacore/caen/Digitizer.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CernlibHist.h"
#   include "caen/Digitizer.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"
#include "Processor.h"

namespace dt5740 {

   /**
    *  @short Monitoring histogram producer for the DT5740
    *
    *         This class is used to produce monitoring histograms for the
    *         DT5740 CAEN digitizer for PAW.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibHist : public virtual dev::CernlibHist,
                       public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( dt5740::CernlibHist )

   public:
      /// Constructor
      CernlibHist();

      /// Initialise the monitoring histograms
      virtual bool initialize( cernlib::HistMgr& hmgr );
      /// Fill the histograms with one event
      virtual bool displayEvent( const ev::Fragment& fragment,
                                 const cernlib::HistMgr& hmgr ) const;

   private:
      mutable Processor m_processor; ///< Object reconstructing the signals
      /// "Map" for the created histograms
      int m_histMap[ NUMBER_OF_GROUPS ][ GroupConfig::CHANNELS_IN_GROUP ][ 2 ];
      mutable caen::Digitizer::EventInfo m_eventInfo; ///< Decoded event info
      mutable caen::Digitizer::EventData16Bit m_eventData; ///< Decoded event data

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class CernlibBist

} // namespace dt5740

#endif // CDA_DEVICES_DT5740_CERNLIBHIST_H
