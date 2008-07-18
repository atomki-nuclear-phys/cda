// Dear emacs, this is -*- c++ -*-
// $Id: Hist.h 39 2008-07-14 17:15:52Z krasznaa $
#ifndef CDA_DEVICES_AD2249A_HIST_H
#define CDA_DEVICES_AD2249A_HIST_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/cernlib/HistMgr.h"
#   include "cdacore/device/Hist.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "cernlib/HistMgr.h"
#   include "device/Hist.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace ad2249a {

   /**
    *  @short AD2249A monitoring histogram producer
    *
    *         This class can be used to create and fill monitoring
    *         histograms with the data read out from a AD2249A CAMAC
    *         ADC device. It uses CERNLIB for allocating and filling
    *         the histograms.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision: 39 $
    * $Date: 2008-07-14 19:15:52 +0200 (Mon, 14 Jul 2008) $
    */
   class Hist : public virtual dev::Hist,
                public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( ad2249a::Hist )

   public:
      /// Constructor
      Hist();

      /// Initialise the monitoring histograms
      virtual bool initialize( cernlib::HistMgr& hmgr );
      /// Fill the histograms with one event
      virtual bool displayEvent( const ev::Fragment& fragment,
                                 const cernlib::HistMgr& hmgr ) const;

   private:
      /// Histogram ID "dictionary"
      /**
       * In CERNLIB the histograms are identified by a single number.
       * This table creates a simple dictionary between the subaddress
       * numbers of this device and the histogram numbers belonging
       * to them. It is filled in the initialize() function.
       */
      int m_histTable[ NUMBER_OF_SUBADDRESSES ];

      mutable msg::Logger m_logger; ///< Object for logging messages

   }; // class Hist

} // namespace ad2249a

#endif // CDA_DEVICES_AD2249A_HIST_H
