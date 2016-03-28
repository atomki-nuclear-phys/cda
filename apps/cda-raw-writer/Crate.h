// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_RAW_WRITER_CRATE_H
#define CDA_APPS_CDA_RAW_WRITER_CRATE_H

// Qt include(s):
#include <QtCore/QString>

// CDA include(s):
#include "device/Crate.h"
#include "device/ICernlibDisk.h"
#include "event/Event.h"
#include "msg/Logger.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QFile )
namespace ev {
   class BinaryStream;
}

/**
 *  @short Namespace for the classes used in cda-raw-writer
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
namespace raw {

   //
   // Make sure that the following Qt classes are available in the
   // DEV namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QFile );

   /**
    *  @short CAMAC crate class for writing RAW files
    *
    *         This specialization of the general dev::Crate class
    *         is used to write the collected events to disk in the
    *         simplest RAW format.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class Crate : public dev::Crate< dev::ICernlibDisk > {

      Q_DECLARE_TR_FUNCTIONS( raw::Crate )

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
      QFile* m_file; ///< Output file
      ev::BinaryStream* m_stream; ///< Stream used for the event writing
      int m_evCounter; ///< Internal event counter
      mutable msg::Logger m_logger; ///< Logger object for the class

   }; // class Crate

} // namespace raw

#endif // CDA_APPS_CDA_RAW_WRITER_CRATE_H
