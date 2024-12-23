
// Qt include(s):
#include <QString>

// CDA include(s):
#include "msg/Message.h"
#include "msg/TextStream.h"

// Local include(s):
#include "TermWindowView.h"

namespace msg {

TermWindowView::TermWindowView(int x, int y, int width, int height)
    : m_mainWindow(nullptr), m_textWindow(nullptr), m_firstMsg(true) {

   // Set up the terminal window:
   setupWin(x, y, (width < 0 ? COLS - x : width),
            (height < 0 ? LINES - y : height));
}

TermWindowView::~TermWindowView() {

   // Make sure that the window is deleted:
   deleteWin();
}

void TermWindowView::addMessage(const Message& message) {

   // Decide whether to show the message:
   if (message.getLevel() < m_minLevel) {
      return;
   }

   // Show the message using the internal function:
   static const bool STORE_IN_QUEUE = true;
   addMessage(message, STORE_IN_QUEUE);

   // Return gracefully:
   return;
}

void TermWindowView::resize(int x, int y, int width, int height) {

   // Setup the window anew:
   setupWin(x, y, width, height);

   // Push all messages from the queue into the view once again:
   std::list<Message>::const_iterator itr = m_msgQueue.begin();
   std::list<Message>::const_iterator end = m_msgQueue.end();
   for (; itr != end; ++itr) {
      static const bool STORE_IN_QUEUE = false;
      addMessage(*itr, STORE_IN_QUEUE);
   }

   // Return gracefully:
   return;
}

void TermWindowView::setupWin(int x, int y, int width, int height) {

   // Start by making sure that the terminal window, if it already exists,
   // is deleted first.
   deleteWin();

   // Create the main window:
   m_mainWindow = ::newwin(height, width, y, x);
   // Draw a "default" box around the window:
   ::box(m_mainWindow, 0, 0);
   // Put a title at the top-left of it:
   ::mvwprintw(m_mainWindow, 0, 1, " Messages ");

   // Create the text window:
   m_textWindow = ::newwin(height - 2, width - 4, y + 1, x + 2);

   // Set the scroll behaviour of the text window:
   ::scrollok(m_textWindow, true);
   ::idlok(m_textWindow, true);
   ::wsetscrreg(m_textWindow, 0, 0);

   // Update the windows on the screen:
   ::wrefresh(m_mainWindow);
   ::wrefresh(m_textWindow);

   // Remember that the next message will be the first one printed:
   m_firstMsg = true;

   // Return gracefully:
   return;
}

void TermWindowView::deleteWin() {

   // If the window doesn't exist, don't do anything:
   if (!m_mainWindow) {
      return;
   }

   // Clear the window border:
   ::wborder(m_mainWindow, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
   // Update the window on the screen, now without its border:
   ::wrefresh(m_mainWindow);
   // Delete the windows:
   ::delwin(m_textWindow);
   ::delwin(m_mainWindow);
   m_mainWindow = nullptr;
   m_textWindow = nullptr;

   // Return gracefully:
   return;
}

void TermWindowView::addMessage(const Message& message, bool storeInQueue) {

   // Format the message into a QString:
   QString text;
   TextStream stream(&text);
   stream << message;

   // Print the line at the current cursor location:
   ::wprintw(m_textWindow, (m_firstMsg ? "%s" : "\n%s"),
             text.toLatin1().constData());

   // Refresh the view:
   ::wrefresh(m_textWindow);

   // The next one won't be the first message anymore:
   m_firstMsg = false;

   // Store the message in the internal queue if needed:
   if (storeInQueue) {
      m_msgQueue.push_back(message);
      if (m_msgQueue.size() > MSG_QUEUE_SIZE) {
         m_msgQueue.pop_front();
      }
   }

   // Return gracefully:
   return;
}

}  // namespace msg
