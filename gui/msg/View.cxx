// $Id$

#include <iostream>

// Qt include(s):
#include <QtCore/QStringList>
#include <QtCore/QSize>
#include <QtGui/QStackedLayout>
#include <QtGui/QTableWidget>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>

// CDA include(s):
#include "msg/Message.h"

// Local include(s):
#include "View.h"

namespace msg {

   //
   // Make sure that the following Qt classes are available in the
   // msg namespace even if Qt has been built in an arbitrary
   // namespace:
   //
   using QT_PREPEND_NAMESPACE( QStringList );
   using QT_PREPEND_NAMESPACE( QSize );
   using QT_PREPEND_NAMESPACE( QTableWidgetItem );
   using QT_PREPEND_NAMESPACE( QHeaderView );

   //
   // Set the values of the constsnts:
   //
   const int View::DEFAULT_TIME_WIDTH   = 160;
   const int View::DEFAULT_SENDER_WIDTH = 150;
   const int View::DEFAULT_LEVEL_WIDTH  = 70;
   const int View::MINIMUM_TEXT_WIDTH   = 300;

   /**
    * The constructor takes care of correctly setting up all all members
    * of the class, which is actually quite a bit of work in this case...
    */
   View::View( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), m_maxMessageCount( 100 ),
        m_sortByTime( true ) {

      // Initialise the maps used to display the messages:
      initMaps();

      // We don't really need a layout, it's just here to make sure the
      // table takes up all available space.
      m_layout = new QStackedLayout( this );

      //
      // Create the main table. Basically turn off any interaction with it
      // from the user side. The user is not supposed to resize the cells,
      // not supposed to edit the cells, re-order the cells, nothing...
      //
      m_table = new QTableWidget();
      m_table->setColumnCount( 4 );
      m_table->setRowCount( 0 );
      m_table->setMinimumSize( 450, 100 );
      m_table->setMaximumSize( 20000, 20000 );
      m_table->setSizePolicy( QSizePolicy::Expanding,
                              QSizePolicy::Expanding );
      m_table->setEditTriggers( QAbstractItemView::NoEditTriggers );
      m_table->setSelectionMode( QAbstractItemView::NoSelection );
      m_table->horizontalHeader()->setMovable( false );
      m_table->horizontalHeader()->setClickable( false );
      m_table->horizontalHeader()->setResizeMode( QHeaderView::Fixed );
      m_table->verticalHeader()->hide();
      m_table->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
      m_table->setHorizontalScrollMode( QAbstractItemView::ScrollPerPixel );
      m_table->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
      m_table->setVerticalScrollMode( QAbstractItemView::ScrollPerItem );

      //
      // Set the names of the columns:
      //
      QStringList headerNames;
      headerNames << "Time" << "Sender" << "Level" << "Message";
      m_table->setHorizontalHeaderLabels( headerNames );

      //
      // Set the widths of the columns:
      //
      m_table->setColumnWidth( 0, DEFAULT_TIME_WIDTH );
      m_table->setColumnWidth( 1, DEFAULT_SENDER_WIDTH );
      m_table->setColumnWidth( 2, DEFAULT_LEVEL_WIDTH );
      m_table->setColumnWidth( 3, MINIMUM_TEXT_WIDTH );

      //
      // The messages will be sorted based on the time when they were
      // created. So let's define the first column to sort with.
      //
      m_table->sortItems( 0 );

      //
      // Finally add the table to the layout:
      //
      m_layout->addWidget( m_table );

   }

   View::~View() {

      //
      // Delete all child objects:
      //
      delete m_table;
      delete m_layout;

   }

   /**
    * @param count Maximum number of messages to show
    */
   void View::setMaximumMessageCount( int count ) {

      m_maxMessageCount = count;
      while( m_table->rowCount() > m_maxMessageCount ) {
         m_table->removeRow( 0 );
      }
      return;

   }

   /**
    * @param value <code>true</code> to sort the messages by time,
    *              <code>false</code> to disable it
    */
   void View::setSortByTime( bool value ) {

      m_sortByTime = value;
      return;

   }

   /**
    * @returns Maximum number of messages to show
    */
   int View::getMaximumMessageCount() const {

      return m_maxMessageCount;

   }

   /**
    * @returns <code>true</code> if the messages are sorted by time,
    *          <code>false</code> if they aren't
    */
   bool View::getSortByTime() const {

      return m_sortByTime;

   }

   /**
    * This function is doing most of the work. It creates a new row in the
    * table and adds the information from the message to this new row in a
    * nicely formatted fashion. It also makes sure that the messages remain
    * time-sorted if the user requested it.
    *
    * @param message The message that should be added to the widget
    */
   void View::addMessage( const Message& message ) {

      //
      // Disable sorting while we add the new message:
      //
      m_table->setSortingEnabled( false );

      //
      // Add a new row to the table. Of course take care of removing
      // the first row if we already hit the configured limit.
      //
      int row = m_table->rowCount();
      if( row == m_maxMessageCount ) {
         m_table->removeRow( 0 );
         --row;
      }
      m_table->insertRow( row );

      //
      // Get the text for the message level:
      //
      std::map< Level, QString >::const_iterator level_it;
      if( ( level_it = m_levelToString.find( message.getLevel() ) ) ==
          m_levelToString.end() ) {
         // There is something wrong with the level setting!
         level_it = m_levelToString.find( INFO );
      }

      //
      // Get the foreground for the message level:
      //
      std::map< Level, QBrush >::const_iterator foreground_it;
      if( ( foreground_it = m_levelToForeground.find( message.getLevel() ) ) ==
          m_levelToForeground.end() ) {
         // There is something wrong with the level setting!
         foreground_it = m_levelToForeground.find( INFO );
      }

      //
      // Get the background for the message level:
      //
      std::map< Level, QBrush >::const_iterator background_it;
      if( ( background_it = m_levelToBackground.find( message.getLevel() ) ) ==
          m_levelToBackground.end() ) {
         // There is something wrong with the level setting!
         background_it = m_levelToBackground.find( INFO );
      }

      //
      // Create the items for the new row:
      //
      QTableWidgetItem* time =
         new QTableWidgetItem( message.getTime().getDate().toString( Qt::SystemLocaleShortDate ) +
                               "/" +
                               message.getTime().getTime().toString( "hh:mm:ss.zzz") );
      time->setForeground( foreground_it->second );
      time->setBackground( background_it->second );
      QTableWidgetItem* sender = new QTableWidgetItem( message.getSender() );
      sender->setForeground( foreground_it->second );
      sender->setBackground( background_it->second );
      QTableWidgetItem* level = new QTableWidgetItem( level_it->second );
      level->setForeground( foreground_it->second );
      level->setBackground( background_it->second );
      QTableWidgetItem* text = new QTableWidgetItem( message.getText() );
      text->setForeground( foreground_it->second );
      text->setBackground( background_it->second );

      //
      // Add the items to the new row. The table now takes control of the
      // items, we don't have to care about deleting them.
      //
      m_table->setItem( row, 0, time );
      m_table->setItem( row, 1, sender );
      m_table->setItem( row, 2, level );
      m_table->setItem( row, 3, text );

      //
      // Enable sorting again, so the table would insert the message in the
      // correct position:
      //
      if( getSortByTime() ) m_table->setSortingEnabled( true );
      m_table->verticalHeader()->resizeSections( QHeaderView::ResizeToContents );
      m_table->scrollToItem( m_table->item( row, 0 ) );

      return;

   }

   void View::showEvent( QShowEvent* ) {

      adjustColumns();
      return;

   }

   void View::resizeEvent( QResizeEvent* ) {

      adjustColumns();
      return;

   }

   /**
    * The class uses multiple maps internally to aid it in presenting the
    * messages in an attractive fashion. These maps are initialised in this
    * function.
    */
   void View::initMaps() {

      m_levelToString[ VERBOSE ] = "VERBOSE";
      m_levelToString[ DEBUG ]   = "DEBUG";
      m_levelToString[ INFO ]    = "INFO";
      m_levelToString[ WARNING ] = "WARNING";
      m_levelToString[ ERROR ]   = "ERROR";
      m_levelToString[ FATAL ]   = "FATAL";
      m_levelToString[ ALWAYS ]  = "ALWAYS";

      m_levelToBackground[ VERBOSE ] = QBrush( Qt::cyan );
      m_levelToBackground[ DEBUG ]   = QBrush( Qt::blue );
      m_levelToBackground[ INFO ]    = QBrush( Qt::white );
      m_levelToBackground[ WARNING ] = QBrush( Qt::yellow );
      m_levelToBackground[ ERROR ]   = QBrush( Qt::red );
      m_levelToBackground[ FATAL ]   = QBrush( Qt::darkRed );
      m_levelToBackground[ ALWAYS ]  = QBrush( Qt::lightGray );

      m_levelToForeground[ VERBOSE ] = QBrush( Qt::black );
      m_levelToForeground[ DEBUG ]   = QBrush( Qt::white );
      m_levelToForeground[ INFO ]    = QBrush( Qt::black );
      m_levelToForeground[ WARNING ] = QBrush( Qt::black );
      m_levelToForeground[ ERROR ]   = QBrush( Qt::white );
      m_levelToForeground[ FATAL ]   = QBrush( Qt::white );
      m_levelToForeground[ ALWAYS ]  = QBrush( Qt::black );

      return;

   }

   /**
    * The widget uses a special policy for resizing. The first 3 columns
    * of the table should never change their width, but the last column
    * (the one showing the message text) should expand to take up all available
    * space. When the size of the widget is small, the function sets the
    * width of the last column to its minimum allowed value.
    */
   void View::adjustColumns() {

      const int time_width   = m_table->columnWidth( 0 );
      const int sender_width = m_table->columnWidth( 1 );
      const int level_width  = m_table->columnWidth( 2 );

      if( m_table->viewport()->width() > time_width + sender_width +
          level_width + MINIMUM_TEXT_WIDTH ) {
         m_table->setColumnWidth( 3, m_table->viewport()->width() - time_width -
                                  sender_width - level_width );
      } else {
         m_table->setColumnWidth( 3, MINIMUM_TEXT_WIDTH );
      }

      return;

   }

} // namespace msg
