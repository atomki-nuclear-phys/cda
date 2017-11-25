// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CURSES_MSG_TERMWINDOWVIEW_H
#define CDA_CURSES_MSG_TERMWINDOWVIEW_H

// Curses include(s):
#include <ncurses.h>

namespace msg {

   /**
    *  @short Terminal window showing the incoming messages
    *
    *         This is a Curses based implementation for a window (in the
    *         terminal) presenting the incoming messages.
    *
    * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
    *
    * $Revision$
    * $Date$
    */
   class TermWindowView {

   public:
      /// Construct a view window on the screen with these dimensions
      TermWindowView( int x = 0, int y = 0, int width = -1, int height = -1 );
      /// Destructor
      ~TermWindowView();

   private:
      /// Function for setting up/updating the window
      void setupWin( int x, int y, int width, int height );
      /// Function for deleting an existing window
      void deleteWin();

      /// The window holding the incoming messages
      WINDOW* m_window;

   }; // class TermWindowView

} // namespace msg

#endif // CDA_CURSES_MSG_TERMWINDOWVIEW_H
