// Dear emacs, this is -*- c++ -*-
// $Id%
#ifndef CDA_DEVICES_V862_CERNLIBHIST_H
#define CDA_DEVICES_V862_CERNLIBHIST_H

// System include(s):
#include <array>

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICernlibHist.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace v862 {

   /// Class filling PAW monitoring histograms from a V862 device
   ///
   /// This class is used to create and fill monitoring histograms in PAW's
   /// common memory during data taking, from data collected from a V862 board.
   ///
   /// @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
   ///
   /// $Revision$
   /// $Date$
   ///
   class CernlibHist : public virtual dev::ICernlibHist,
                       public virtual Device {

      // In order to get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( v862::CernlibHist )

   public:
      /// Default constructor
      CernlibHist();

      /// @name Functions inherited from dev::ICernlibHist
      /// @{

      /// Initialise the monitoring histograms
      virtual bool initialize( cernlib::HistMgr& hmgr );
      /// Fill the histograms with one event
      virtual bool displayEvent( const ev::Fragment& fragment,
                                 const cernlib::HistMgr& hmgr ) const;

      /// @}

   private:
      /// "Map" for the created histograms
      std::array< int, NUMBER_OF_CHANNELS > m_histMap;

      /// Message logging object
      mutable msg::Logger m_logger;

   }; // class CernlibHist

} // namespace v862

#endif // CDA_DEVICES_V862_CERNLIBHIST_H
