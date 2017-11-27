// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_CURSES_MSG_TERMWINDOWVIEW_H
#define CDA_CURSES_MSG_TERMWINDOWVIEW_H

// System include(s):
#include <list>

// Curses include(s):
#include <ncurses.h>

// Qt include(s):
#include <QObject>

// CDA include(s):
#include "msg/Level.h"
#include "msg/Message.h"

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
      /// Resize the window on in the terminal
      void resize( int x, int y, int width, int height );

   private:
      /// Function for setting up/updating the window
      void setupWin( int x, int y, int width, int height );
      /// Function for deleting an existing window
      void deleteWin();

      /// Internal function for adding a message to the window
      void addMessage( const Message& message, bool storeInQueue );

      /// The main window
      WINDOW* m_mainWindow;
      /// The text (scrolling) window
      WINDOW* m_textWindow;

      /// The minimum message level to show
      Level m_minLevel;

      /// Internal flag showing whether we're printing the first message on the
      /// screen or not
      bool m_firstMsg;

      /// Queue of the last 50 messages received by the view. To be able to
      /// (re-)show them correctly in case the view gets resized
      std::list< Message > m_msgQueue;
      /// The number of messages to queue
      static const size_t MSG_QUEUE_SIZE = 50;

   }; // class TermWindowView

} // namespace msg

#endif // CDA_CURSES_MSG_TERMWINDOWVIEW_H
