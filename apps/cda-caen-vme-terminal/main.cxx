// $Id$

// Curses include(s):
#include <ncurses.h>

// CDA include(s):
#include "msg/PanelView.h"

int main() {

   ::initscr();
   ::cbreak();
   ::noecho();
   ::refresh();

   {
      msg::PanelView messages;
      ::getch();
   }

   ::endwin();
   return 0;
}
