// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_GLOMEM_WRITER_CRATE_H
#define CDA_APPS_CDA_GLOMEM_WRITER_CRATE_H

// CDA include(s):
#include "cernlib/HistMgr.h"
#include "device/Crate.h"
#include "device/ICernlibHist.h"
#include "event/Event.h"
#include "msg/Logger.h"

/**
 *  @short Namespace holding classes used by cda-glomem-writer
 *
 *         Since all the applications use a class named Crate, I have
 *         to put all these objects in different namespaces not to confuse
 *         the documentation system. Even though these classes would
 *         never see each other anyway...
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
namespace glomem {

/**
 *  @short Class representing the CAMAC crate for histogram writing
 *
 *         This class is used to display the monitoring histograms from
 *         all the devices in the CAMAC crate.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
class Crate : public dev::Crate<dev::ICernlibHist> {

   Q_DECLARE_TR_FUNCTIONS(glomem::Crate)

public:
   /// Constructor
   Crate();

   /// Initialise the monitoring histograms
   bool initialize();
   /// Add one event to the monitoring histograms
   bool displayEvent(const ev::Event& event);

private:
   cernlib::HistMgr m_hmgr;       ///< Interface for handling histograms
   mutable msg::Logger m_logger;  ///< Message logging object

};  // class Crate

}  // namespace glomem

#endif  // CDA_APPS_CDA_GLOMEM_WRITER_CRATE_H
