// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_T2228A_HIST_H
#define CDA_DEVICES_T2228A_HIST_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Hist.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Hist.h"
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
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Hist : public virtual dev::Hist,
                public virtual Device {

   public:
      /// Constructor
      Hist();

      /// Initialise the monitoring histograms
      virtual bool initialize( unsigned int& counter );
      /// Fill the histograms with one event
      virtual bool displayEvent( const ev::Fragment& fragment );

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

} // namespace t2228a

#endif // CDA_DEVICES_T2228A_HIST_H
