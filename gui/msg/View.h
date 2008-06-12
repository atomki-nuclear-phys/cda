// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_GUI_MSG_VIEW_H
#define CDA_GUI_MSG_VIEW_H

// STL include(s):
#include <map>

// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QWidget>
#include <QtGui/QBrush>

// CDA include(s):
#include "msg/Level.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QStackedLayout )
QT_FORWARD_DECLARE_CLASS( QTableWidget )

namespace msg {

   // Forward declaration(s):
   class Message;

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QWidget );
   using QT_PREPEND_NAMESPACE( QString );
   using QT_PREPEND_NAMESPACE( QStackedLayout );
   using QT_PREPEND_NAMESPACE( QTableWidget );
   using QT_PREPEND_NAMESPACE( QBrush );

   /**
    *  @short Widget capable of showing messages
    *
    *         This Qt widget can be used to show messages in a graphical
    *         application. It uses a specially configured QTableWidget
    *         to present the messages.
    *
    * @author Attila Krasznahorkay Jr.
    *
    * $Revision$
    * $Date$
    */
   class View : public QWidget {

      Q_OBJECT

   public:
      /// Standard QWidget-type constructor
      View( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
      /// Destructor
      ~View();

      /// Get the maximal number of messages that can be shown
      int  getMaximumMessageCount() const;
      /// Get if the messages should be sorted
      bool getSortByTime() const;

   public slots:
      /// Insert a new message into the widget
      void addMessage( const Message& message );
      /// Set the number of messages to show
      void setMaximumMessageCount( int count );
      /// Set if the messages should be sorted
      void setSortByTime( bool value );

   protected:
      /// Function taking care of the widget resize policy
      virtual void showEvent( QShowEvent* event );
      /// Function taking care of the widget resize policy
      virtual void resizeEvent( QResizeEvent* event );

   private:
      /// Function initialising the internal maps
      void initMaps();
      /// Function adjusting the width of the table columns
      void adjustColumns();

      /// Default width of the "time" column
      static const int DEFAULT_TIME_WIDTH;
      /// Default width of the "sender" column
      static const int DEFAULT_SENDER_WIDTH;
      /// Default width of the "level" column
      static const int DEFAULT_LEVEL_WIDTH;
      /// Minimum width of the "text" column
      static const int MINIMUM_TEXT_WIDTH;

      QStackedLayout* m_layout; ///< The layout used by the widget
      QTableWidget*   m_table;  ///< The table showing the messages

      /// Map to print the message level
      std::map< Level, QString > m_levelToString;
      /// Map to set the background of the messages
      std::map< Level, QBrush >  m_levelToBackground;
      /// Map to set the foreground of the messages
      std::map< Level, QBrush >  m_levelToForeground;

      int  m_maxMessageCount; ///< Maximum number of shown messages
      bool m_sortByTime;      ///< Flag for sorting the messages

   }; // class View

} // namespace msg

#endif // CDA_GUI_MSG_VIEW_H
