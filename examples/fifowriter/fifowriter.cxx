// $Id$
/**
 * @example fifowriter.cxx
 *
 *          Example showing how to write data to a FIFO.
 *          This example opens a local FIFO file
 *          (<strong>./testfifo</strong>) and writes a msg::Message
 *          object in it using msg::BinaryStream. It kind of
 *          demonstrates how complex objects can be sent from
 *          process to process using FIFOs.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// Qt include(s):
#include <QtCore/QtGlobal>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/fifo/Fifo.h"
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/msg/BinaryStream.h"
#   include "cdacore/event/Event.h"
#   include "cdacore/event/BinaryStream.h"
#else
#   include "fifo/Fifo.h"
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "msg/BinaryStream.h"
#   include "event/Event.h"
#   include "event/BinaryStream.h"
#endif

int main() {

   //
   // Set the destination of the messages:
   //
   msg::Sender::addAddress( msg::Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::VERBOSE );
   msg::Logger logger( "fifowriter" );

   //
   // Open the FIFO:
   //
   Fifo fifo( "../fifos/histFifo" );
   if( fifo.open() ) {
      logger << msg::INFO << "FIFO file opened" << msg::endmsg;
   } else {
      logger << msg::ERROR << "FIFO file could not be opened"
             << msg::endmsg;
      return 1;
   }

   //
   // Send a msg::Message over it:
   //
   /*
   msg::Message message( "fifowriter", "Test message this is..." );
   msg::BinaryStream msgstream( &fifo );
   msgstream << message;

   logger << msg::INFO << "msg::Message sent over FIFO" << msg::endmsg;
   */

   //
   // Send an ev::Event over it:
   //
   ev::Event event;
   ev::Fragment fragment1, fragment2;

   fragment1.setCrateNumber( 2 );
   fragment1.setModuleNumber( 10 );
   fragment1.addDataWord( 0xf0 );
   fragment1.addDataWord( 0xf00 );

   fragment2.setCrateNumber( 1 );
   fragment2.setModuleNumber( 15 );
   fragment2.addDataWord( 0xc0 );
   fragment2.addDataWord( 0xc00 );

   event.addFragment( fragment1 );
   event.addFragment( fragment2 );

   ev::BinaryStream evstream( &fifo );
   evstream << event;
   fifo.flush();

   logger << msg::INFO << "ev::Event sent over FIFO" << msg::endmsg;

   return 0;

}
