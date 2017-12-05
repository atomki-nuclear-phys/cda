// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_EVENT_TEXTSTREAM_H
#define CDA_CORE_EVENT_TEXTSTREAM_H

// Qt include(s):
#include <QTextStream>

// CDA include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QString )
QT_FORWARD_DECLARE_CLASS( QByteArray )

namespace ev {

   // Forward declaration(s):
   class Event;
   class Fragment;

   //
   // Make sure that the following Qt classes are available in the
   // ev namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QTextStream );
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QByteArray );

   /**
    *  @short Class for printing the contents of Event objects
    *
    *         This class is intended to help in debugging the communications
    *         between the components of CDA by allowing to print the
    *         contents of Event objects in an easy-to-read format. Of
    *         course in a real data acquisition time should not be spent
    *         with printing the events, this is only for developing the
    *         code...
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    */
   class CDACORE_EXPORT TextStream : public QTextStream {

   public:
      /// Constructor for handling a generic device
      TextStream( QIODevice* device = 0 );
      /// Constructor for handling system file descriptors
      TextStream( FILE* fileHandle );
      /// Constructor for handling strings
      TextStream( QString* string );
      /// Constructor for handling byte arrays
      TextStream( QByteArray* array );

      /// Operator for printing an event
      TextStream& operator<< ( const Event& event );
      /// Operator for printing an event fragment
      TextStream& operator<< ( const Fragment& fragment );
      /// Operator for outputting a QString
      TextStream& operator<< ( const QString& text );

   }; // class TextStream

} // namespace ev

#endif // CDA_CORE_EVENT_TEXTSTREAM_H
