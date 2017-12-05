
// STL include(s):
#include <iostream>

// Qt include(s):
#include <QTcpSocket>

// Local include(s):
#include "Sender.h"
#include "BinaryStream.h"
#include "TextStream.h"

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QTcpSocket );

   //
   // Instantiate the static members:
   //
   Sender*                Sender::m_instance  = 0;
   std::vector< Address > Sender::m_addresses = std::vector< Address >();

   /**
    * @returns The only instance of this class
    */
   Sender* Sender::instance() {

      if( ! m_instance ) {
         m_instance = new Sender();
      }

      return m_instance;
   }

   /**
    * @param address An address where the messages should be sent
    */
   void Sender::addAddress( const Address& address ) {

      m_addresses.push_back( address );
      return;
   }

   /**
    * @returns All the addresses where messages will be sent
    */
   const std::vector< Address >& Sender::getAddresses() {

      return m_addresses;
   }

   /**
    * @returns The message type above which messages are sent
    */
   Level Sender::getMinLevel() const {

      return m_minLevel;
   }

   /**
    * @param level The message type above which messages are sent
    */
   void Sender::setMinLevel( Level level ) {

      m_minLevel = level;
      return;
   }

   /**
    * This is the main function for sending off messages to the server(s).
    * In case a server can not be reached, it prints some error
    * messages to the console.
    *
    * @param message The message that is sent to the configured server(s)
    */
   void Sender::send( const Message& message ) const {

      //
      // Only send messages above the given level:
      //
      if( message.getLevel() >= m_minLevel ) {

         for( std::vector< Address >::const_iterator address = m_addresses.begin();
              address != m_addresses.end(); ++address ) {

            //
            // Open a connection to the message server:
            //
            QTcpSocket socket;
            socket.connectToHost( address->getHost(), address->getPort(),
                                  QIODevice::WriteOnly );
            if( ! socket.waitForConnected() ) {
               printError( message );
               continue;
            }
            if( ! socket.open( QIODevice::WriteOnly ) ) {
               printError( message );
               continue;
            }

            //
            // Send the message:
            //
            BinaryStream out( &socket );
            out << message;

            //
            // Close the connection:
            //
            socket.disconnectFromHost();
            socket.waitForDisconnected();
         }

         //
         // Print the message even if no servers have been defined yet:
         //
         if( ! m_addresses.size() ) {
            //
            // Print a single warning:
            //
            static bool firstcall = true;
            if( firstcall ) {
               std::cout << tr( "* No message server set up yet.\n"
                                "* Messages that could not be sent will be\n"
                                "* printed to the console."  ).toLatin1().constData()
                         << std::endl << std::endl;
               firstcall = false;
            }

            //
            // Print the message to the standard output:
            //
            TextStream out( stdout );
            out << message << "\n";
         }

      }

      return;
   }

   Sender::Sender()
      : m_minLevel( INFO ) {

   }

   /**
    * @param message The message that could not be sent to one of the servers
    */
   void Sender::printError( const Message& message ) const {

      //
      // Print a single warning:
      //
      static bool firstcall = true;
      if( firstcall ) {
         std::cout << tr( "* A Message Server could not be contacted!\n"
                          "* Messages that could not be sent will be\n"
                          "* printed to the console." ).toLatin1().constData()
                   << std::endl << std::endl;
         firstcall = false;
      }

      //
      // Print the message to the standard output:
      //
      TextStream out( stdout );
      out << message << "\n";

      return;
   }

} // namespace msg
