// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_STAT_SERVER_STATSERVERWINDOW_H
#define CDA_APPS_CDA_STAT_SERVER_STATSERVERWINDOW_H

// Qt include(s):
#include <QMainWindow>

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QDockWidget)
QT_FORWARD_DECLARE_CLASS(QAction)
namespace cdastat {
class Server;
class MDIView;
}  // namespace cdastat

/**
 *  @short Main window of cda-stat-server
 *
 *         This is the main window of the CDA Statistics Server. It presents
 *         the status of the entire CDA DAQ system to the user in a coherent
 *         way.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
class StatServerWindow : public QMainWindow {

   Q_OBJECT

public:
   /// Constructor
   StatServerWindow();
   /// Destructor
   ~StatServerWindow();

private slots:
   /// Slot showing a window about cda-stat-server
   void aboutStatServerSlot();
   /// Slot showing a window about CDA
   void aboutCDASlot();
   /// Port handling the changes in the port settings
   void portChangedSlot();

private:
   /// Function creating the menus of the window
   void createMenus();
   /// Function creating the dock widgets of the window
   void createDockWidgets();

   cdastat::Server* m_server;  ///< Statistics server
   cdastat::MDIView* m_view;   ///< Statistics presenter widget

   QSpinBox* m_portEdit;     ///< Listening port editor
   QDockWidget* m_portDock;  ///< Dock widget for the listening port editor

   QAction*
       m_showPortDock;  ///< Action showing/hiding the port editor dock widget

};  // class StatServerWindow

#endif  // CDA_APPS_CDA_STAT_SERVER_STATSERVERWINDOW_H
