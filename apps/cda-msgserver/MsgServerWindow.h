// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_MSGSERVER_MSGSERVERWINDOW_H
#define CDA_APPS_CDA_MSGSERVER_MSGSERVERWINDOW_H

// Qt include(s):
#include <QtGui/QMainWindow>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QDockWidget )
QT_FORWARD_DECLARE_CLASS( QSpinBox )
QT_FORWARD_DECLARE_CLASS( QGroupBox )
QT_FORWARD_DECLARE_CLASS( QHBoxLayout )
QT_FORWARD_DECLARE_CLASS( QLabel )
QT_FORWARD_DECLARE_CLASS( QLineEdit )
QT_FORWARD_DECLARE_CLASS( QAction )
QT_FORWARD_DECLARE_CLASS( QString )
QT_FORWARD_DECLARE_CLASS( QCheckBox )
QT_FORWARD_DECLARE_CLASS( QComboBox )
namespace msg {
   class Server;
   //   class TableView;
   class TextView;
} // namespace msg

/**
 *  @short Message Server window
 *
 *         This is the top level window of cda-msgserver. It is a simple
 *         extension of QMainWindow, adding a msg::View object as the
 *         center widget.
 *
 *         The window allows to modify a few properties of msg::View and
 *         msg::Server, the ability to modify more parameters could be
 *         added later on.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
class MsgServerWindow : public QMainWindow {

   Q_OBJECT

public:
   /// Standard constructor
   MsgServerWindow();
   /// Destructor
   ~MsgServerWindow();

private slots:
   /// Slot handling changes to the listening TCP/IP port
   void portChangedSlot();
   /// Slot handling changes to the number of shown message lines
   void shownLinesChangedSlot();
   /// Slot handling changes to file writing requests
   void outputFileWritingChangedSlot( bool value );
   /// Slot handling changes to the output file name
   void outputFileNameChangedSlot( const QString& text );
   /// Slot handling changes to the minimum message level
   void minMsgLevelChangedSlot( const QString& value );
   /// Slot showing a window about Qt
   void aboutQtSlot();
   /// Slot showing a window about CDA
   void aboutCDASlot();
   /// Slot showing a window about this application
   void aboutMsgServerSlot();

private:
   /// Function creating the menus for the window
   void createMenus();
   /// Function creating the dock widgets for the window
   void createDockWidgets();

   msg::Server*    m_server; ///< The server listening for incoming messages
   //   msg::TableView* m_view;   ///< The widget showing the messages
   msg::TextView*  m_view;   ///< The widget showing the messages

   QSpinBox*    m_portEdit; ///< TCP port modification widget
   QDockWidget* m_portDock; ///< Dock widget for the TCP port modification

   QSpinBox*    m_shownLinesEdit; ///< Maximum line number modification widget
   QDockWidget* m_shownLinesDock; ///< Dock widget for the maximum line modification

   /*
   QCheckBox*   m_timeOrderMessagesEdit;
   QDockWidget* m_timeOrderMessagesDock;
   */

   QComboBox*   m_minMsgLevelEdit; ///< Minimum level of shown messages
   QDockWidget* m_minMsgLevelDock; ///< Dock widget for the msg. level modifications

   QGroupBox*   m_outputFileBox;    ///< Box holding the output file related widgets
   QHBoxLayout* m_outputFileLayout; ///< Layout of the output file box
   QLabel*      m_outputFileLabel;  ///< A text label in the output file box
   QLineEdit*   m_outputFileEdit;   ///< Output file name modification widget
   QDockWidget* m_outputFileDock;   ///< Dock widget for the output file modifications

   QAction* m_quitAction; ///< Action quiting the application

   QAction* m_showPortDock;       ///< Action handling the port dock visibility
   QAction* m_showShownLinesDock; ///< Action handling the shown lines dock visibility
   QAction* m_showOutputFileDock; ///< Action handling the output file dock visibility
   //   QAction* m_showOrderingDock;   ///< Action handling the ordering dock visibility
   QAction* m_showMsgLevelDock;   ///< Action handling the msg. level dock visibility

   QAction* m_aboutQtAction;        ///< Action showing a window about Qt
   QAction* m_aboutCDAAction;       ///< Action showing a window about CDA
   QAction* m_aboutMsgServerAction; ///< Action showing a window about this application

}; // class MsgServerWindow

#endif // CDA_APPS_CDA_MSGSERVER_MSGSERVERWINDOW_H
