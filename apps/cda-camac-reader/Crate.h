// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAMAC_READER_CRATE_H
#define CDA_APPS_CDA_CAMAC_READER_CRATE_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/Readout.h"
#   include "cdacore/device/Crate.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/Readout.h"
#   include "device/Crate.h"
#   include "event/Event.h"
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
namespace camac {
   class Crate;
} // namespace camac

/**
 *  @short Namespace holding classes used by cda-camac-reader
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
namespace reader {

   /**
    *  @short Class representing the CAMAC crate for event readout
    *
    *         As all the crate-like classes, this is a specialisation
    *         of dev::Crate. The base class takes care of reading the
    *         configuration of the devices in the crate, this class
    *         only has to take care of performing the data readout.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Crate : public dev::Crate< dev::Readout > {

   public:
      /// Constructor
      Crate();
      /// Destructor
      ~Crate();

      /// Function initialising all the devices in the crate
      bool initialize( camac::Crate& crate ) const;
      /// Function reading out one event from the crate
      ev::Event readEvent( camac::Crate& crate ) const;

   private:
      mutable msg::Logger m_logger; ///< Message logging object

   }; // class Crate

} // namespace reader

#endif // CDA_APPS_CDA_CAMAC_READER_CRATE_H
