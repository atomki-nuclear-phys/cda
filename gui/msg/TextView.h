//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2025 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_GUI_MSG_TEXTVIEW_H
#define CDA_GUI_MSG_TEXTVIEW_H

// CDA include(s):
#include "msg/Level.h"

// Qt include(s).
#include <QWidget>

// STL include(s):
#include <memory>

namespace msg {

// Forward declaration(s):
class Message;

/**
 *  @short Widget capable of showing messages
 *
 *         This Qt widget can be used to show messages in a graphical
 *         application. It uses a QTextEdit widget to present the messages.
 *
 *         The idea was that this should be faster than msg::TableView,
 *         but it turns out, it's only marginally faster... (Even though
 *         the class is much simpler.)
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class TextView : public QWidget {

   Q_OBJECT

public:
   /// Standard QWidget-type constructor
   TextView(QWidget* parent = 0, Qt::WindowFlags flags = Qt::WindowFlags());
   /// Destructor
   ~TextView();

   /// Get the maximal number of messages that can be shown
   int getMaximumMessageCount() const;
   /// Get the type under which messages should not be shown
   Level getMinimumShownLevel() const;

public slots:
   /// Insert a new message into the widget
   void addMessage(const Message& message);
   /// Set the number of messages to show
   void setMaximumMessageCount(int count);
   /// Set the type under which messages should not be shown
   void setMinimumShownLevel(Level level);

private:
   /// Internal data type
   struct Impl;
   /// Internal data
   std::unique_ptr<Impl> m_impl;

};  // class TextView

}  // namespace msg

#endif  // CDA_GUI_MSG_TEXTVIEW_H
