// $Id$
/**
 * @example msgserver.cxx
 *
 *          This executable starts an instance of msg::Server on
 *          port 49700 of localhost. The server writes all received
 *          messages in a file called "messages.log".
 *
 *          It is to be used together with the msgclient executable.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */

// STL include(s):
#include <iostream>

// Qt include(s):
#include <QtCore/QCoreApplication>

// Local include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Server.h"
#else
#   include "msg/Server.h"
#endif

int main( int argc, char* argv[] ) {

   // To use the Qt signal-slot framework, such an object
   // has to be instantiated:
   QCoreApplication app( argc, argv );

   // Create and start the server:
   msg::Server server;
   if( ! server.listen( msg::Address( "0.0.0.0", 49700 ) ) ) {
      std::cerr << "The Message Server could not be started!" << std::endl;
      return 1;
   } else {
      std::cout << "The Message Server is running." << std::endl << std::endl;
   }

   // Run until being stopped externally:
   return app.exec();

}