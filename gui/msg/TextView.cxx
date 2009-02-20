// $Id$

// Qt include(s):
#include <QtCore/QString>
#include <QtGui/QStackedLayout>
#include <QtGui/QTextEdit>
#include <QtGui/QTextDocument>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Message.h"
#   include "cdacore/msg/TextStream.h"
#else
#   include "msg/Message.h"
#   include "msg/TextStream.h"
#endif

// Local include(s):
#include "TextView.h"

namespace msg {

   /**
    * The constructor creates the QTextEdit object used to present the
    * messages and sets it up to show the messages in a reasonable way.
    */
   TextView::TextView( QWidget* parent, Qt::WindowFlags flags )
      : QWidget( parent, flags ), /*m_maxMessageCount( 100 ),*/
        m_minShownLevel( VERBOSE ) {

      // Init the internal maps:
      initMaps();

      // We don't really need a layout, it's just here to make sure the
      // text edit widget takes up all available space.
      m_layout = new QStackedLayout( this );

      // Create the main widget. It is configured to use fixed-width fonts
      // and to only show a maximum of a 100 messages at a time. (The latter
      // can be changed by the code using the class.)
      m_edit = new QTextEdit();
      m_edit->setMinimumSize( 450, 100 );
      m_edit->setMaximumSize( 20000, 20000 );
      m_edit->setSizePolicy( QSizePolicy::Expanding,
                             QSizePolicy::Expanding );
      m_edit->setReadOnly( true );
      m_edit->setFontFamily( "courier" );
#ifdef Q_OS_DARWIN
      //      m_edit->setFontPointSize( 12 );
#else
      m_edit->setFontPointSize( 10 );
#endif // Q_OS_MAC
      m_edit->setLineWrapMode( QTextEdit::NoWrap );
      m_edit->document()->setMaximumBlockCount( 100 );

      m_layout->addWidget( m_edit );

   }

   TextView::~TextView() {

      delete m_edit;
      delete m_layout;

   }

   /**
    * @returns The maximum number of messages that can be shown
    */
   int TextView::getMaximumMessageCount() const {

      return m_edit->document()->maximumBlockCount();

   }

   /**
    * @returns The minimum message level that is shown
    */
   Level TextView::getMinimumShownLevel() const {

      return m_minShownLevel;

   }

   /**
    * This function adds the new message as (a) new line(s) to the main
    * widget. It uses msg::TextStream to format the messages, only adding
    * some color-coding on top of this.
    *
    * @param message The message that should be appended to the view
    */
   void TextView::addMessage( const Message& message ) {

      //
      // Return right away if the message should not be shown:
      //
      if( message.getLevel() < m_minShownLevel ) return;

      //
      // Format the message into a QString:
      //
      QString msg_text;
      TextStream stream( &msg_text );
      stream << message;

      //
      // Get the foreground for the message level:
      //
      std::map< Level, QColor >::const_iterator foreground_it;
      if( ( foreground_it = m_levelToForeground.find( message.getLevel() ) ) ==
          m_levelToForeground.end() ) {
         // There is something wrong with the level setting!
         foreground_it = m_levelToForeground.find( INFO );
      }

      //
      // Get the background for the message level:
      //
      std::map< Level, QColor >::const_iterator background_it;
      if( ( background_it = m_levelToBackground.find( message.getLevel() ) ) ==
          m_levelToBackground.end() ) {
         // There is something wrong with the level setting!
         background_it = m_levelToBackground.find( INFO );
      }

      m_edit->setTextColor( foreground_it->second );
      m_edit->setTextBackgroundColor( background_it->second );
      m_edit->append( msg_text );

      return;

   }

   void TextView::setMaximumMessageCount( int count ) {

      m_edit->document()->setMaximumBlockCount( count );
      return;

   }

   void TextView::setMinimumShownLevel( Level level ) {

      m_minShownLevel = level;
      return;

   }

   /**
    * The class uses internal maps to color-code the messages based on
    * their message level. These maps are configured in this function.
    */
   void TextView::initMaps() {

      m_levelToBackground[ VERBOSE ] = QColor( Qt::cyan );
      m_levelToBackground[ DEBUG ]   = QColor( Qt::blue );
      m_levelToBackground[ INFO ]    = QColor( Qt::white );
      m_levelToBackground[ WARNING ] = QColor( Qt::yellow );
      m_levelToBackground[ ERROR ]   = QColor( Qt::red );
      m_levelToBackground[ FATAL ]   = QColor( Qt::darkRed );
      m_levelToBackground[ ALWAYS ]  = QColor( Qt::lightGray );

      m_levelToForeground[ VERBOSE ] = QColor( Qt::black );
      m_levelToForeground[ DEBUG ]   = QColor( Qt::white );
      m_levelToForeground[ INFO ]    = QColor( Qt::black );
      m_levelToForeground[ WARNING ] = QColor( Qt::black );
      m_levelToForeground[ ERROR ]   = QColor( Qt::white );
      m_levelToForeground[ FATAL ]   = QColor( Qt::white );
      m_levelToForeground[ ALWAYS ]  = QColor( Qt::black );

      return;

   }

} // namespace msg
