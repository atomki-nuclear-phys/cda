// Dear emacs, this is -*- c++ -*-
#ifndef CDA_GUI_MSG_TEXTVIEW_H
#define CDA_GUI_MSG_TEXTVIEW_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QColor>
#include <QWidget>

// CDA include(s):
#include "msg/Level.h"

// Local include(s):
#include "../common/Export.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QStackedLayout)
QT_FORWARD_DECLARE_CLASS(QTextEdit)

namespace msg {

// Forward declaration(s):
class Message;

//
// Make sure that the following Qt classes are available in the
// msg namespace even if Qt has been built in an arbitrary
// namespace:
//
using QT_PREPEND_NAMESPACE(QWidget);
using QT_PREPEND_NAMESPACE(QStackedLayout);
using QT_PREPEND_NAMESPACE(QTextEdit);
using QT_PREPEND_NAMESPACE(QColor);

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
class CDAGUI_EXPORT TextView : public QWidget {

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
   /// Function initialising the internal maps
   void initMaps();

   QStackedLayout* m_layout;  ///< The layout used by the widget
   QTextEdit* m_edit;         ///< The text field showing the messages

   /// Map to set the background of the messages
   std::map<Level, QColor> m_levelToBackground;
   /// Map to set the foreground of the messages
   std::map<Level, QColor> m_levelToForeground;

   Level m_minShownLevel;  ///< Level under which messages are not shown

};  // class TextView

}  // namespace msg

#endif  // CDA_GUI_MSG_TEXTVIEW_H
