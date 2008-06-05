// $Id$
/**
 * @example fifowriter.cxx
 *
 *          Example showing how to write data to a FIFO.
 *          This example opens a local FIFO file
 *          (<strong>./testfifo</strong>) and writes a MSG::Message
 *          object in it using MSG::BinaryStream. It kind of
 *          demonstrates how complex objects can be sent from
 *          process to process using FIFOs.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// CDA include(s):
#include "msg/Sender.h"
#include "msg/Logger.h"
#include "msg/BinaryStream.h"
#include "fifo/Fifo.h"

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
   Fifo fifo( "./testfifo" );
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
   msg::Message message( "fifowriter", "Test message this is..." );
   msg::BinaryStream stream( &fifo );
   stream << message;

   logger << msg::INFO << "msg::Message sent over FIFO" << msg::endmsg;

   return 0;

}
