// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CURSES_MSG_TERMWINDOWVIEW_H
#define CDA_CURSES_MSG_TERMWINDOWVIEW_H

// Curses include(s):
#include <ncurses.h>

// Qt include(s):
#include <QObject>

// CDA include(s):
#include "msg/Level.h"

namespace msg {

   // Forward declaration(s):
   class Message;

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
   class TermWindowView : public QObject {

      Q_OBJECT

   public:
      /// Construct a view window on the screen with these dimensions
      TermWindowView( int x = 0, int y = 0, int width = -1, int height = -1 );
      /// Destructor
      ~TermWindowView();

   public slots:
      /// Insert a new message into the window
      void addMessage( const Message& message );

   private:
      /// Function for setting up/updating the window
      void setupWin( int x, int y, int width, int height );
      /// Function for deleting an existing window
      void deleteWin();

      /// The main window
      WINDOW* m_mainWindow;
      /// The text (scrolling) window
      WINDOW* m_textWindow;

      /// The minimum message level to show
      Level m_minLevel;

      /// Internal flag showing whether we're printing the first message on the
      /// screen or not
      bool m_firstMsg;

   }; // class TermWindowView

} // namespace msg

#endif // CDA_CURSES_MSG_TERMWINDOWVIEW_H
