// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T2228A_CERNLIBHIST_H
#define CDA_DEVICES_T2228A_CERNLIBHIST_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/CernlibHist.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/CernlibHist.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace t2228a {

   /**
    *  @short T2228A monitoring histogram producer
    *
    *         This class can be used to create and fill monitoring
    *         histograms with the data read out from a T2228A CAMAC
    *         TDC device. It uses CERNLIB for allocating and filling
    *         the histograms.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class CernlibHist : public virtual dev::CernlibHist,
                       public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( t2228a::Hist )

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

} // namespace t2228a

#endif // CDA_DEVICES_T2228A_CERNLIBHIST_H
