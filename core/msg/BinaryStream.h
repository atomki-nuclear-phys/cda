// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_MSG_BINARYSTREAM_H
#define CDA_CORE_MSG_BINARYSTREAM_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QDataStream>

// Local include(s):
#include "Level.h"
#include "Message.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QByteArray )

// Check the Qt version used for compilation:
#if QT_VERSION < QT_VERSION_CHECK( 4, 0, 0 )
#  error "MSG::BinaryStream needs Qt >= 4.0.0"
#endif

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // MSG namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QByteArray );

   /**
    *  @short Class helping in message communication
    *
    *         I'm following the Qt design here. This extra class
    *         helps in the transmission of messages between separate
    *         components. It does so by providing "serialisation" for
    *         the Message objects so that they can be transmitted in
    *         various ways.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class BinaryStream : public QDataStream {

   public:
      /// Constructor for handling a generic device
      BinaryStream( QIODevice* device = 0 );
      /// Constructor for handling an array
      BinaryStream( QByteArray* array,
                    QIODevice::OpenMode openMode = QIODevice::ReadWrite );

      /// Operator for sending a message
      BinaryStream& operator<< ( const Message& message );
      /// Operator for receiving a message
      BinaryStream& operator>> ( Message& message );

   private:
      /// Function that initialises the internal maps at construction
      void initMaps();

      std::map< Level, quint16 > m_forwardMap; ///< Map used for message level serialisation
      std::map< quint16, Level > m_reverseMap; ///< Map used for message level serialisation

   }; // class BinaryStream

} // namespace msg

#endif // CDA_CORE_MSG_BINARYSTREAM_H
