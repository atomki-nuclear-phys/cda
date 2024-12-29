//
// ATOMKI Common Data Acquisition
//
// (c) 2008-2024 ATOMKI, Debrecen, Hungary
//
// Apache License Version 2.0
//
#ifndef CDA_APPS_CDA_CAMAC_DAQ_MAINWINDOW_H
#define CDA_APPS_CDA_CAMAC_DAQ_MAINWINDOW_H

// CDA include(s).
#include "msg/Server.h"
#include "msg/TextView.h"
#include "simple_daq/CamacReaderRunner.h"
#include "simple_daq/GlomemWriterRunner.h"
#include "simple_daq/HBookWriterRunner.h"
#include "simple_daq/RawWriterRunner.h"
#include "simple_daq/RootWriterRunner.h"
#include "simple_daq/Statistics.h"

// Qt include(s).
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>

/**
 *  @short Main (and only) window of cda-caen-daq
 *
 *         This is the main window of cda-caen-daq. It shows widgets that
 *         make it easy to start the relevant applications for a CAEN Digitizer
 *         data acquisition, with the click of a button. It is even smart enough
 *         to start these applications with all the correct command line
 *         options.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 */
class MainWindow : public QMainWindow {

   Q_OBJECT

public:
   /// Constructor with configuration file name and output verbosity setting
   MainWindow(const QString& confFileName = "",
              msg::Level verbosity = msg::INFO);

private slots:
   /// Slot for specifying a configuration file
   void readConfigSlot();
   /// Slot for displaying a window about this application
   void aboutCaenDAQSlot();
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
   /// Widget to draw on
   QWidget m_centralWidget;

   /// Layout of the window
   QGridLayout m_layout;

   /// Message server listening to the child processes
   msg::Server m_msgServer;
   /// Widget showing the incoming messages
   msg::TextView m_msgView;

   /// Widget showing DAQ statistics
   simple_daq::Statistics m_statistics;
   /// Widget controlling cda-camac-reader
   simple_daq::CamacReaderRunner m_camacReader;
   /// Widget controlling cda-glomem-writer
   simple_daq::GlomemWriterRunner m_glomemWriter;
   /// Widget controlling cda-hbook-writer
   simple_daq::HBookWriterRunner m_hbookWriter;
   /// Widget controlling cda-root-writer
   simple_daq::RootWriterRunner m_rootWriter;
   /// Widget controlling cda-raw-writer
   simple_daq::RawWriterRunner m_rawWriter;

   /// Description of the extra event receivers
   QLabel m_eventReceiversLabel;
   /// Widget specifying the extra event receivers
   QComboBox m_eventReceivers;
   /// Button for removing an event receiver
   QPushButton m_removeEventReceiver;
   /// Address of the currently selected event receiver
   QString m_currentEventReceiver;

   /// Description of the extra statistics receivers
   QLabel m_statReceiversLabel;
   /// Widget specifying the extra statistics receivers
   QComboBox m_statReceivers;
   /// Button for removing an statistics receiver
   QPushButton m_removeStatReceiver;
   /// Address of the currently selected stat. receiver
   QString m_currentStatReceiver;

};  // class MainWindow

#endif  // CDA_APPS_CDA_CAMAC_DAQ_MAINWINDOW_H
