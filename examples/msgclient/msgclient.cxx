// $Id$
/**
 * @example msgclient.cxx
 *
 *          This executable sends off >100 messages to a message
 *          server that is expected to listen on port 49700 of
 *          <i>localhost</i>. It is very interesting to note that
 *          the messages don't necessarily have to arrive to the
 *          server in the same order that they were sent in. On
 *          linux I actually saw the messages arriving in the same
 *          order, but Mac OS X seems to do things in a parallelized
 *          way, and the messages ended up in a semi-random order.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// STL include(s):
#include <iomanip>

// Local include(s):
#include "msg/Sender.h"
#include "msg/Logger.h"

int main() {

   // Set the destination of the messages:
   msg::Sender::addAddress( msg::Address( "127.0.0.1", 49700 ) );

   // Create the logger class:
   msg::Logger logger( "msgclient" );

   // Send some messages very quickly. Observe the order of the
   // received messages on the server side!
   for( int i = 0; i < 100; ++i ) {
      logger << msg::ERROR << "A test message: " << i << msg::endmsg;
   }

   // Demonstation of what msg::Logger can absorbe:
   logger << msg::INFO << "This is a multiline\n"
          << "message." << std::endl << std::endl
          << " 0xff = " << std::dec << 0xff << std::endl
          << " 1234 = 0x" << std::setw( 6 ) << std::setfill( '0' )
          << std::hex << 1234 << msg::endmsg;

   return 0;

}
