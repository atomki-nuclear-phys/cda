// $Id$

// Local include(s):
#include "TermWindowView.h"

namespace msg {

   TermWindowView::TermWindowView( int x, int y, int width, int height )
      : m_window( nullptr ) {

      // Set up the terminal window:
      setupWin( x, y, ( width < 0 ? COLS - x : width ),
                ( height < 0 ? LINES - y : height ) );
   }

   TermWindowView::~TermWindowView() {

      // Make sure that the window is deleted:
      deleteWin();
   }

   void TermWindowView::setupWin( int x, int y, int width, int height ) {

      // Start by making sure that the terminal window, if it already exists,
      // is deleted first.
      deleteWin();

      // Create the main window:
      m_window = ::newwin( height, width, y, x );
      // Draw a "default" box around it:
      ::box( m_window, 0 , 0 );
      // Put a title at the top-left of the window:
      ::mvwprintw( m_window, 0, 1, " Messages " );

      // Update the window on the screen:
      ::wrefresh( m_window );

      // Return gracefully:
      return;
   }

   void TermWindowView::deleteWin() {

      // If the window doesn't exist, don't do anything:
      if( ! m_window ) {
         return;
      }

      // Clear the window border:
      ::wborder( m_window, ' ', ' ', ' ',' ',' ',' ',' ',' ' );
      // Update the window on the screen, now without its border:
      ::wrefresh( m_window );
      // Delete the window:
      ::delwin( m_window );

      // Return gracefully:
      return;
   }

} // namespace msg
