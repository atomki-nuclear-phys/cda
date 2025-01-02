//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_APPS_CDA_HBOOK_WRITER_CRATE_H
#define CDA_APPS_CDA_HBOOK_WRITER_CRATE_H

// CDA include(s):
#include "cernlib/NTupleMgr.h"
#include "device/Crate.h"
#include "device/ICernlibDisk.h"
#include "event/Event.h"
#include "msg/Logger.h"

// Qt include(s):
#include <QtCore/QString>

/**
 *  @short Namespace for the classes used in cda-hbook-writer
 *
 *         To keep the class names nice and short, we introduce a
 *         dedicated namespace for the classes defined specifically
 *         for the application.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
namespace hbook {

/**
 *  @short CAMAC crate class for writing HBOOK files
 *
 *         This specialization of the general dev::Crate class
 *         is used to write the collected events to disk using
 *         the CERNLIB libraries.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class Crate : public dev::Crate<dev::ICernlibDisk> {

   Q_DECLARE_TR_FUNCTIONS(hbook::Crate)

public:
   /// Default constructor
   Crate();

   /// Create and initialize the output file
   StatusCode initialize(const QString& fileName);
   /// Write one event to the output file
   StatusCode writeEvent(const ev::Event& event);
   /// Finalize and close the output file
   StatusCode finalize();

private:
   cernlib::NTupleMgr m_nmgr;  ///< Object actually handling the ntuple
   msg::Logger m_logger;       ///< Logger object for the class

};  // class Crate

}  // namespace hbook

#endif  // CDA_APPS_CDA_HBOOK_WRITER_CRATE_H
