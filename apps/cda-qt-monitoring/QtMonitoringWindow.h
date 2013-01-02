// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_QT_MONITORING_QTMONITORINGWINDOW_H
#define CDA_APPS_CDA_QT_MONITORING_QTMONITORINGWINDOW_H

// Qt include(s):
#include <QMainWindow>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/device/QtHist.h"
#   include "cdacore/device/Crate.h"
#   include "cdacore/msg/Logger.h"
#else
#   include "device/QtHist.h"
#   include "device/Crate.h"
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QSpinBox )
QT_FORWARD_DECLARE_CLASS( QDockWidget )
QT_FORWARD_DECLARE_CLASS( QAction )
QT_FORWARD_DECLARE_CLASS( QMdiArea )
namespace ev {
   class EventServer;
}

/**
 *  @short Main window of cda-qt-monitoring
 *
 *         This is the main window of CDA's Qt Monitoring application.
 *         It can monitor a CDA DAQ session when CERNLIB is not available
 *         for some reason.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class QtMonitoringWindow : public QMainWindow,
   public dev::Crate< dev::QtHist > {

   Q_OBJECT

public:
   /// Constructor
   QtMonitoringWindow();
   /// Destructor
   ~QtMonitoringWindow();

private slots:
   /// Slot handling the reading of a configuration
   void readConfigSlot();
   /// Slot showing a window about cda-stat-server
   void aboutQtMonitoringSlot();
   /// Slot showing a window about CDA
   void aboutCDASlot();
   /// Port handling the changes in the port settings
   void portChangedSlot();
   /// Slot handling the incoming events
   void processEvents();

private:
   /// Function reading an XML configuration file
   bool readXMLConfig( const QString& filename );
   /// Function reading a binary configuration file
   bool readBinaryConfig( const QString& filename );
   /// Function creating the menus of the window
   void createMenus();
   /// Function creating the dock widgets of the window
   void createDockWidgets();

   ev::EventServer* m_server; ///< Event server, receiving the events
   QMdiArea*        m_view; ///< Area where the monitoring is shown

   QSpinBox* m_portEdit; ///< Listening port editor
   QDockWidget* m_portDock; ///< Dock widget for the listening port editor

   QAction* m_showPortDock; ///< Action showing/hiding the port editor dock widget

   mutable msg::Logger m_logger; ///< Message logger object

}; // class StatServerWindow

#endif // CDA_APPS_CDA_QT_MONITORING_QTMONITORINGWINDOW_H
