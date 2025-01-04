//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2025 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//

// Local include(s):
#include "TextView.h"

// CDA include(s):
#include "msg/Message.h"
#include "msg/TextStream.h"

// Qt include(s):
#include <QStackedLayout>
#include <QString>
#include <QTextDocument>
#include <QTextEdit>

// System include(s).
#include <utility>

namespace {

/// Decide whether a dark theme is active
bool shouldApplyDarkTheme() {
   const QPalette defaultPalette;
   return defaultPalette.color(QPalette::WindowText).lightness() >
          defaultPalette.color(QPalette::Window).lightness();
}

}  // namespace

namespace msg {

struct TextView::Impl {

   /// Constructor
   Impl(QWidget& parent) : m_layout{&parent}, m_edit{&parent} {

      m_layout.addWidget(&m_edit);
   }

   /// The layout used by the widget
   QStackedLayout m_layout;
   /// The text field showing the messages
   QTextEdit m_edit;

   /// Map to set the foreground and background color of the messages
   std::map<Level, std::pair<QColor, QColor> > m_levelColor;

   /// Level under which messages are not shown
   Level m_minShownLevel;
};

/**
 * The constructor creates the QTextEdit object used to present the
 * messages and sets it up to show the messages in a reasonable way.
 */
TextView::TextView(QWidget* parent, Qt::WindowFlags flags)
    : QWidget(parent, flags), m_impl{new Impl{*this}} {

   // Set up the text view widget.
   m_impl->m_edit.setReadOnly(true);
   m_impl->m_edit.setFontFamily("courier");
   m_impl->m_edit.setLineWrapMode(QTextEdit::NoWrap);
   m_impl->m_edit.document()->setMaximumBlockCount(100);

   // Set the colour of the different messages.
   m_impl->m_levelColor[VERBOSE] = {QColor(Qt::cyan), QColor(Qt::black)};
   m_impl->m_levelColor[DEBUG] = {QColor(Qt::blue), QColor(Qt::white)};
   m_impl->m_levelColor[INFO] = {
       m_impl->m_edit.textBackgroundColor(),
       shouldApplyDarkTheme() ? QColor(Qt::white) : QColor(Qt::black)};
   m_impl->m_levelColor[WARNING] = {QColor(Qt::yellow), QColor(Qt::black)};
   m_impl->m_levelColor[ERROR] = {QColor(Qt::red), QColor(Qt::white)};
   m_impl->m_levelColor[FATAL] = {QColor(Qt::darkRed), QColor(Qt::white)};
   m_impl->m_levelColor[ALWAYS] = {QColor(Qt::lightGray), QColor(Qt::black)};
}

TextView::~TextView() {}

/**
 * @returns The maximum number of messages that can be shown
 */
int TextView::getMaximumMessageCount() const {

   return m_impl->m_edit.document()->maximumBlockCount();
}

/**
 * @returns The minimum message level that is shown
 */
Level TextView::getMinimumShownLevel() const {

   return m_impl->m_minShownLevel;
}

/**
 * This function adds the new message as (a) new line(s) to the main
 * widget. It uses msg::TextStream to format the messages, only adding
 * some color-coding on top of this.
 *
 * @param message The message that should be appended to the view
 */
void TextView::addMessage(const Message& message) {

   //
   // Return right away if the message should not be shown:
   //
   if (message.getLevel() < m_impl->m_minShownLevel) {
      return;
   }

   //
   // Format the message into a QString:
   //
   QString msg_text;
   TextStream stream(&msg_text);
   stream << message;

   //
   // Get the foreground/background color for the message level.
   //
   auto color_it = m_impl->m_levelColor.find(message.getLevel());
   if (color_it == m_impl->m_levelColor.end()) {
      // There is something wrong with the level setting!
      color_it = m_impl->m_levelColor.find(INFO);
   }

   // Add the message.
   m_impl->m_edit.setTextBackgroundColor(color_it->second.first);
   m_impl->m_edit.setTextColor(color_it->second.second);
   m_impl->m_edit.append(msg_text);
}

void TextView::setMaximumMessageCount(int count) {

   m_impl->m_edit.document()->setMaximumBlockCount(count);
}

void TextView::setMinimumShownLevel(Level level) {

   m_impl->m_minShownLevel = level;
}

}  // namespace msg
