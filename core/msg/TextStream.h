// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_TEXTSTREAM_H
#define CDA_CORE_MSG_TEXTSTREAM_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QTextStream>

// Local include(s):
#include "Message.h"
#include "Level.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QString )
QT_FORWARD_DECLARE_CLASS( QByteArray )

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "msg::TextStream needs Qt >= 4.0.0"
#endif

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
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class TextStream : public QTextStream {

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
