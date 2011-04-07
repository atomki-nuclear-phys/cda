// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_HBOOK_WRITER_CRATE_H
#define CDA_APPS_CDA_HBOOK_WRITER_CRATE_H

// Qt include(s):
#include <QtCore/QtGlobal>
#include <QtCore/QString>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Crate.h"
#   include "cdacore/device/Disk.h"
#   include "cdacore/cernlib/NTupleMgr.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Crate.h"
#   include "device/Disk.h"
#   include "cernlib/NTupleMgr.h"
#   include "event/Event.h"
#   include "msg/Logger.h"
#endif

/**
 *  @short Namespace for the classes used in cda-hbook-writer
 *
 *         To keep the class names nice and short, we introduce a
 *         dedicated namespace for the classes defined specifically
 *         for the application.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
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
    *
    * $Revision$
    * $Date$
    */
   class Crate : public dev::Crate< dev::Disk > {

      Q_DECLARE_TR_FUNCTIONS( hbook::Crate )

   public:
      /// Default constructor
      Crate();
      /// Destructor, closing the output file
      ~Crate();

      /// Create and initialize the output file
      bool initialize( const QString& fileName );
      /// Write one event to the output file
      bool writeEvent( const ev::Event& event );
      /// Finalize and close the output file
      bool finalize();

   private:
      cernlib::NTupleMgr m_nmgr; ///< Object actually handling the ntuple
      mutable msg::Logger m_logger; ///< Logger object for the class

   }; // class Crate

} // namespace hbook

#endif // CDA_APPS_CDA_HBOOK_WRITER_CRATE_H
