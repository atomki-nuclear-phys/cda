// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T4300B_CERNLIBHIST_H
#define CDA_DEVICES_T4300B_CERNLIBHIST_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QCoreApplication>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/cernlib/HistMgr.h"
#   include "cdacore/device/CernlibHist.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "cernlib/HistMgr.h"
#   include "device/CernlibHist.h"
#   include "msg/Logger.h"
#endif

// Local include(s):
#include "Device.h"

namespace t4300b {

   /**
    *  @short T4300B monitoring histogram producer
    *
    *         This class can be used to create and fill monitoring
    *         histograms with the data read out from a T4300B CAMAC
    *         QDC device. It uses CERNLIB for allocating and filling
    *         the histograms.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CernlibHist : public virtual dev::CernlibHist,
                       public virtual Device {

      Q_DECLARE_TR_FUNCTIONS( t2228a::CernlibHist )

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

} // namespace t4300b

#endif // CDA_DEVICES_T4300B_CERNLIBHIST_H