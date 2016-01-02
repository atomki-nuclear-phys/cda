// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_DAQ_CONFIG_CRATE_H
#define CDA_DAQ_CONFIG_CRATE_H

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/ICernlibHist.h"
#   include "cdacore/device/Crate.h"
#else
#   include "device/ICernlibHist.h"
#   include "device/Crate.h"
#endif

namespace conf {

   /**
    *  @short Crate class used to "translate" between configuration formats
    *
    *         The configuration server always sends the configuration
    *         information to its clients in a binary format. To be able
    *         to read in the configuration from an XML file and then send
    *         it in binary format, we need such a helper crate object
    *         to translate between these things.
    *
    *         This class should not be used outside the conf:: namespace!
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class Crate : public dev::Crate< dev::ICernlibHist > {

   public:
      /// Constructor
      Crate();

   }; // class Crate

} // namespace conf

#endif // CDA_DAQ_CONFIG_CRATE_H
