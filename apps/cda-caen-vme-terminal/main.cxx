// $Id$

// System include(s):
#include <sstream>

// Curses include(s):
#include <ncurses.h>

// CDA include(s):
#include "msg/Message.h"
#include "msg/TermWindowView.h"

int main() {

   ::initscr();
   ::cbreak();
   ::noecho();
   ::refresh();
   ::curs_set( 0 );

   {
      msg::TermWindowView messages( 0, 0, COLS, LINES );
      for( int i = 0; i < 50; ++i ) {
         std::ostringstream msgNumber;
         msgNumber << i + 1 << " 12341234123412341234123412341234123412341234123412341234123412341234123412341234123412341234";
         messages.addMessage( msg::Message( "test", ( "Test INFO message " +
                                                      msgNumber.str() ).c_str(),
                                            msg::INFO ) );
         messages.addMessage( msg::Message( "test", ( "Test WARNING message " +
                                                      msgNumber.str() ).c_str(),
                                            msg::WARNING ) );
      }
      int ch = 0;
      while( ( ch = ::getch() ) == KEY_RESIZE ) {
         messages.resize( 0, 0, COLS, LINES );
      }
   }

   ::endwin();
   return 0;
}
