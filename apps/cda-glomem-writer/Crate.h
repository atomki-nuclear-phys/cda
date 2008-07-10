// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_GLOMEM_WRITER_CREATE_H
#define CDA_APPS_CDA_GLOMEM_WRITER_CREATE_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Hist.h"
#   include "cdacore/device/Crate.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Hist.h"
#   include "device/Crate.h"
#   include "event/Event.h"
#   include "msg/Logger.h"
#endif

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
   class Crate : public dev::Crate< dev::Hist > {

   public:
      /// Constructor
      Crate();
      /// Destructor
      ~Crate();

      /// Initialise the monitoring histograms
      bool initialize();
      /// Add one event to the monitoring histograms
      bool displayEvent( const ev::Event& event );

   private:
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Crate

} // namespace glomem

#endif // CDA_APPS_CDA_GLOMEM_WRITER_CREATE_H