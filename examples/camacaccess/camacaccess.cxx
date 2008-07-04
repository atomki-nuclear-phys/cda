// $Id$
/**
 * @example camacaccess.cxx
 *
 *          This example plays with the camac::Crate class a bit. Nothing
 *          serious as at this time the class has not been tested in a
 *          real life situation yet.
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
#   include "cdacore/msg/Sender.h"
#   include "cdacore/msg/Logger.h"
#   include "cdacore/camac/Crate.h"
#else
#   include "msg/Sender.h"
#   include "msg/Logger.h"
#   include "camac/Crate.h"
#endif

int main() {

   //
   // Set up message logging in the example:
   //
   msg::Sender::addAddress( msg::Address( "127.0.0.1", 49700 ) );
   msg::Sender::instance()->setMinLevel( msg::VERBOSE );
   msg::Logger logger( "camacaccess" );

   // Construct a Crate object:
   camac::Crate crate;

   // Try what happens when reading from a closed CAMAC crate:
   crate.readWord( 10, 5, 0 );

   // Open the CAMAC crate:
   if( ! crate.open() ) {
      logger << msg::FATAL << "Failed to open CAMAC crate" << msg::endmsg;
      return 1;
   } else {
      logger << msg::INFO << "Opened CAMAC crate" << msg::endmsg;
   }

   //
   // Perform some basic read and write operations:
   //
   crate.readWord( 10, 5, 0 );
   crate.readLong( 15, 2, 2 );

   crate.writeWord( 10, 0, 10, 20 );
   crate.writeLong( 15, 0, 12, 30 );

   // Close the CAMAC crate:
   if( ! crate.close() ) {
      logger << msg::FATAL << "Failed to close CAMAC crate" << msg::endmsg;
      return 1;
   } else {
      logger << msg::INFO << "Closed CAMAC crate" << msg::endmsg;
   }

   // Return gracefully:
   return 0;

}
