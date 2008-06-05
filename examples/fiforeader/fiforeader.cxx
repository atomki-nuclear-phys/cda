// $Id$
/**
 * @example fiforeader.cxx
 *
 *          Example showing how to read data from a FIFO.
 *          This small example can be used to test the functioning of
 *          the Fifo class. It opens a FIFO file called
 *          <strong>./testfifo</strong> and tries to read msg::Message
 *          objects from it. When successful, it prints the message
 *          that it read.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// System include(s):
#include <cstdio>

// CDA include(s):
#include "msg/Sender.h"
#include "msg/Logger.h"
#include "msg/BinaryStream.h"
#include "msg/TextStream.h"
#include "fifo/Fifo.h"

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
   // Try to read a complex (msg::Message) object from the FIFO
   // until the program terminates:
   //
   msg::BinaryStream stream( &fifo );
   for( ; ; ) {

      msg::TextStream output( stdout );

      msg::Message message;
      stream >> message;
      output << message << "\n";

   }

   return 0;

}
