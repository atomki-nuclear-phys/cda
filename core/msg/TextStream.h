// Dear emacs, this is -*- c++ -*-
#ifndef CDA_CORE_MSG_TEXTSTREAM_H
#define CDA_CORE_MSG_TEXTSTREAM_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QTextStream>

// CDA include(s):
#include "../common/Export.h"

// Local include(s):
#include "Message.h"
#include "Level.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QString )
QT_FORWARD_DECLARE_CLASS( QByteArray )

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QTextStream );
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QByteArray );

   /**
    *  @short Class for creating formatted message output
    *
    *         This class can be used to write a message in a
    *         user readable format to a number of destinations.
    *         It can be used to write a message to the console,
    *         to a file, etc. It should be used whenever a message
    *         is to be visualised.
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

      /// Operator for outputting a message
      TextStream& operator<<( const Message& message );
      /// Operator for outputting a QString
      TextStream& operator<<( const QString& text );

   private:
      /// Internal function for setting up the object
      void initMap();

      std::map< Level, QString > m_levelMap; ///< Translation for message levels

   }; // class TextStream

} // namespace msg

#endif // CDA_CORE_MSG_TEXTSTREAM_H
