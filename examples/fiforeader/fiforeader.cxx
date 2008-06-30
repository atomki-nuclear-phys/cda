// $Id$
/**
 * @example fiforeader.cxx
 *
 *          Example showing how to read data from a FIFO.
 *          This small example can be used to test the functioning of
 *          the Fifo class. It opens a FIFO file called
 *          <strong>./testfifo</strong> and tries to read msg::Message
 *          and ev::Event objects from it. When successful, it prints the
 *          message and event that it read.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// System include(s):
#include <cstdio>

// Qt include(s):
#include <QtCore/QGlobalStatic>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/fifo/Fifo.h"
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/msg/BinaryStream.h"
#   include "cdacore/msg/TextStream.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/BinaryStream.h"
#   include "cdacore/event/TextStream.h"
#else
#   include "fifo/Fifo.h"
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "msg/BinaryStream.h"
#   include "msg/TextStream.h"
#   include "event/Event.h"
#   include "event/BinaryStream.h"
#   include "event/TextStream.h"
#endif

int main() {

   //
   // Set the destination of the messages:
   //
   msg::Sender::addAddress( msg::Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::VERBOSE );
   msg::Logger logger( "fiforeader" );

   //
   // Open the fifo:
   //
   Fifo fifo( "./testfifo" );
   if( fifo.open() ) {
      logger << msg::INFO << "FIFO file opened" << msg::endmsg;
   } else {
      logger << msg::ERROR << "FIFO file could not be opened"
             << msg::endmsg;
      return 1;
   }
   logger << msg::INFO << "Waiting for incoming data..." << msg::endmsg;

   //
   // Try to read a msg::Message object followed by an ev::Event
   // object from the FIFO until the program terminates:
   //
   for( ; ; ) {

      msg::BinaryStream msgstream( &fifo );
      msg::TextStream msgoutput( stdout );

      msg::Message message;
      msgstream >> message;
      msgoutput << message << "\n";

      ev::BinaryStream evstream( &fifo );
      ev::TextStream evoutput( stdout );

      ev::Event event;
      evstream >> event;
      evoutput << event << "\n";

   }

   return 0;

}
