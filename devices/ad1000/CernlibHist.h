// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DEVICES_AD1000_CERNLIBHIST_H
#define CDA_DEVICES_AD1000_CERNLIBHIST_H

// Qt include(s):
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "device/ICernlibHist.h"
#include "msg/Logger.h"

// Local include(s):
#include "Device.h"

namespace ad1000 {

/**
 *  @short AD1000 monitoring histogram producer
 *
 *         This class can be used to create and fill monitoring
 *         histograms with the data read out from an AD1000 CAMAC
 *         ADC device. It uses CERNLIB for allocating and filling
 *         the histograms.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class CernlibHist : public virtual dev::ICernlibHist, public virtual Device {

   // To get the tr() function:
   Q_DECLARE_TR_FUNCTIONS(ad1000::CernlibHist)

public:
   /// Constructor
   CernlibHist();

   /// Initialise the monitoring histograms
   virtual bool initialize(cernlib::HistMgr& hmgr);
   /// Fill the histograms with one event
   virtual bool displayEvent(const ev::Fragment& fragment,
                             const cernlib::HistMgr& hmgr) const;

private:
   /// Histogram ID "dictionary"
   /**
    * In CERNLIB the histograms are identified by a single number.
    * This number represents the histogram created for this device.
    * It is filled in the initialize() function.
    */
   int m_histNumber;

   mutable msg::Logger m_logger;  ///< Object for logging messages

};  // class CernlibHist

}  // namespace ad1000

#endif  // CDA_DEVICES_AD1000_CERNLIBHIST_H
