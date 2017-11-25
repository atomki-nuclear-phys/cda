// $Id$

// Curses include(s):
#include <ncurses.h>

// CDA include(s):
#include "msg/TermWindowView.h"

int main() {

   ::initscr();
   ::cbreak();
   ::noecho();
   ::refresh();

   {
      msg::TermWindowView messages;
      ::getch();
   }

   ::endwin();
   return 0;
}
