// $Id$

// System include(s):
#include <sstream>

// Curses include(s):
#include <ncurses.h>

// Qt include(s):
#include <QCoreApplication>
#include <QObject>

// CDA include(s):
#include "msg/Server.h"
#include "msg/Message.h"
#include "msg/TermWindowView.h"

int main( int argc, char* argv[] ) {

   // Set up a Qt console application:
   QCoreApplication app( argc, argv );

   // Start the message server:
   msg::Server server;
   server.listen( Address( "0.0.0.0", 49700 ) );

   ::initscr();
   ::cbreak();
   ::noecho();
   ::refresh();
   ::curs_set( 0 );

   {
      msg::TermWindowView messages( 0, 0, COLS, LINES );
      QObject::connect( &server,
                        SIGNAL( messageAvailable( const Message& ) ),
                        &messages, SLOT( addMessage( const Message& ) ) );
      app.exec();
   }

   ::endwin();
   return 0;
}
