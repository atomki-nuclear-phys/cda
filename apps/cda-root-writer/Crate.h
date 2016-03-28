// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_ROOT_WRITER_CRATE_H
#define CDA_APPS_CDA_ROOT_WRITER_CRATE_H

// Qt include(s):
#include <QtCore/QString>

// CDA include(s):
#include "device/Crate.h"
#include "device/IRootDisk.h"
#include "root/NTupleMgr.h"
#include "event/Event.h"
#include "msg/Logger.h"

/**
 *  @short Namespace for the classes used in cda-root-writer
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
namespace root {

   /**
    *  @short CAMAC crate class for writing ROOT files
    *
    *         This specialization of the general dev::Crate class
    *         is used to write the collected events to disk using
    *         the ROOT libraries.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Crate : public dev::Crate< dev::IRootDisk > {

      Q_DECLARE_TR_FUNCTIONS( root::Crate )

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
      root::NTupleMgr m_nmgr; ///< Object actually handling the ntuple
      mutable msg::Logger m_logger; ///< Logger object for the class

   }; // class Crate

} // namespace root

#endif // CDA_APPS_CDA_ROOT_WRITER_CRATE_H
