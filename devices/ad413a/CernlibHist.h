// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD413A_CERNLIBHIST_H
#define CDA_DEVICES_AD413A_CERNLIBHIST_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/ICernlibHist.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/ICernlibHist.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace ad413a {

   /**
    *  @short AD413A monitoring histogram producer
    *
    *         This class can be used to create and fill monitoring
    *         histograms with the data read out from a AD413A CAMAC
    *         ADC device. It uses CERNLIB for allocating and filling
    *         the histograms.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibHist : public virtual dev::ICernlibHist,
                       public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( ad2249a::CernlibHist )

   public:
      /// Constructor
      CernlibHist();

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

   }; // class CernlibHist

} // namespace ad413a

#endif // CDA_DEVICES_AD413A_CERNLIBHIST_H
