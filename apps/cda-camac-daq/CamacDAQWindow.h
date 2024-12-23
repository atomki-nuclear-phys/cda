// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAMAC_DAQ_CAMACDAQWINDOW_H
#define CDA_APPS_CDA_CAMAC_DAQ_CAMACDAQWINDOW_H

// Qt include(s):
#include <QMainWindow>

// CDA include(s):
#include "msg/Logger.h"

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS(QWidget)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QPushButton)
namespace msg {
class Server;
class TextView;
}  // namespace msg
namespace simple_daq {
class Statistics;
class CamacReaderRunner;
class GlomemWriterRunner;
class HBookWriterRunner;
class RootWriterRunner;
class RawWriterRunner;
}  // namespace simple_daq

/**
 *  @short Main (and only) window of cda-camac-daq
 *
 *         This is the main window of cda-camac-daq. It shows widgets that make
 *         it easy to start the cda-camac-reader, cda-glomem-writer and
 * cda-hbook-writer applications with the click of a button. It is even smart
 * enough to start these applications with all the correct command line options.
 *
 * @author Attila Krasznahorkay Jr.
 *
 * $Revision$
 * $Date$
 */
class CamacDAQWindow : public QMainWindow {

   Q_OBJECT

public:
   /// Constructor with configuration file name and output verbosity setting
   CamacDAQWindow(const QString& confFileName = "",
                  msg::Level verbosity = msg::INFO);
   /// Destructor
   ~CamacDAQWindow();

private slots:
   /// Slot for specifying a configuration file
   void readConfigSlot();
   /// Slot for displaying a window about this application
   void aboutCamacDAQSlot();
   /// Slot for displaying a window about CDA
   void aboutCDASlot();

   /// Slot for handling changes to the current event receiver
   void eventReceiverIndexChangedSlot(const QString& text);
   /// Slot for handling changes to the event receiver's address
   void eventReceiverTextChangedSlot(const QString& text);
   /// Slot for removing the current event receiver
   void removeEventReceiverSlot();

   /// Slot for handling changes to the current statistics receiver
   void statReceiverIndexChangedSlot(const QString& text);
   /// Slot for handling changes to the statistics receiver's address
   void statReceiverTextChangedSlot(const QString& text);
   /// Slot for removing the current event receiver
   void removeStatReceiverSlot();

private:
   /// Private function drawing the menus of the window
   void drawMenus();

   QWidget* m_centralWidget;  ///< Widget to draw on

   msg::Server*
       m_msgServer;  ///< Message server listening to the child processes
   msg::TextView* m_msgView;  ///< Widget showing the incoming messages

   simple_daq::Statistics* m_statistics;  ///< Widget showing DAQ statistics
   simple_daq::CamacReaderRunner*
       m_camacReader;  ///< Widget controlling cda-camac-reader
   simple_daq::GlomemWriterRunner*
       m_glomemWriter;  ///< Widget controlling cda-glomem-writer
   simple_daq::HBookWriterRunner*
       m_hbookWriter;  ///< Widget controlling cda-hbook-writer
   simple_daq::RootWriterRunner*
       m_rootWriter;  ///< Widget controlling cda-root-writer
   simple_daq::RawWriterRunner*
       m_rawWriter;  ///< Widget controlling cda-raw-writer

   QLabel* m_eventReceiversLabel;  ///< Description of the extra event receivers
   QComboBox*
       m_eventReceivers;  ///< Widget specifying the extra event receivers
   QPushButton*
       m_removeEventReceiver;       ///< Button for removing an event receiver
   QString m_currentEventReceiver;  ///< Address of the currently selected event
                                    ///< receiver

   QLabel*
       m_statReceiversLabel;  ///< Description of the extra statistics receivers
   QComboBox*
       m_statReceivers;  ///< Widget specifying the extra statistics receivers
   QPushButton*
       m_removeStatReceiver;  ///< Button for removing an statistics receiver
   QString m_currentStatReceiver;  ///< Address of the currently selected stat.
                                   ///< receiver

   mutable msg::Logger m_logger;  ///< Private message logger

};  // class CamacDAQWindow

#endif  // CDA_APPS_CDA_CAMAC_DAQ_CAMACDAQWINDOW_H
