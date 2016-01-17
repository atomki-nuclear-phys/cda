// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CORE_EVENT_BINARYSTREAM_H
#define CDA_CORE_EVENT_BINARYSTREAM_H

// Qt include(s):
#include <QtCore/QDataStream>
#include <QtCore/QCoreApplication>

// CDA include(s):
#include "../msg/Logger.h"
#include "../common/Export.h"

// Local include(s):
#include "IncompleteEvent.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QIODevice )
QT_FORWARD_DECLARE_CLASS( QByteArray )

namespace ev {

   //
   // Make sure that the following Qt classes are available in the
   // ev namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QDataStream );
   using QT_PREPEND_NAMESPACE( QIODevice );
   using QT_PREPEND_NAMESPACE( QByteArray );

   // Local forward declaration(s):
   class Event;
   class Fragment;

   /**
    *  @short Class for transmitting CAMAC events in binary form
    *
    *         Objects of this class can be used to send and receive
    *         full event records over some form of binary connection.
    *         It can be used to send the events over a FIFO, over a
    *         network connection, or even to read/write them from/to
    *         a physical file.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class CDACORE_EXPORT BinaryStream : public QDataStream {

      // To get the tr() function:
      Q_DECLARE_TR_FUNCTIONS( ev::BinaryStream )

   public:
      /// Constructor operating on a binary device
      BinaryStream( QIODevice* device = 0 );
      /// Constructor operating on a byte-array
      BinaryStream( QByteArray* array,
                    QIODevice::OpenMode openMode = QIODevice::ReadWrite );

      /// Operator "serialising" a full CAMAC event
      BinaryStream& operator<< ( const Event& event );
      /// Operator "de-serialising" a full CAMAC event
      BinaryStream& operator>> ( Event& event );

   private:
      /// Operator "serialising" a CAMAC event fragment
      BinaryStream& operator<< ( const Fragment& fragment );
      /// Operator "de-serialising" a CAMAC event fragment
      BinaryStream& operator>> ( Fragment& fragment );

      /// Function making sure that the socket has data available
      bool ensureDataAvailable( qint64 minBytes );
      /// Function making sure that the socket sends all data
      bool ensureDataSent( qint64 maxBytes = 50000 );

      mutable msg::Logger m_logger; ///< Message logger object

   }; // class BinaryStream

} // namespace ev

#endif // CDA_CORE_EVENT_BINARYSTREAM_H
