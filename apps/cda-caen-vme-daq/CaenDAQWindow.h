// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAEN_VME_DAQ_CAENDAQWINDOW_H
#define CDA_APPS_CDA_CAEN_VME_DAQ_CAENDAQWINDOW_H

// System include(s):
#include <memory>

// Qt include(s):
#include <QComboBox>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>

// CDA include(s):
#include "msg/Logger.h"
#include "msg/Server.h"
#include "msg/TextView.h"
#include "simple_daq/CaenVmeReaderRunner.h"
#include "simple_daq/GlomemWriterRunner.h"
#include "simple_daq/HBookWriterRunner.h"
#include "simple_daq/RawWriterRunner.h"
#include "simple_daq/RootWriterRunner.h"
#include "simple_daq/Statistics.h"

/**
 *  @short Main (and only) window of cda-caen-vme-daq
 *
 *         This is the main window of cda-caen-vme-daq. It shows widgets that
 *         make it easy to start the relevant applications for a CAEN VME data
 *         acquisition, with the click of a button. It is even smart enough to
 *         start these applications with all the correct command line options.
 *
 * @author Attila Krasznahorkay <Attila.Krasznahorkay@cern.ch>
 *
 * $Revision$
 * $Date$
 */
class CaenDAQWindow : public QMainWindow {

   Q_OBJECT

public:
   /// Constructor with configuration file name and output verbosity setting
   CaenDAQWindow(const QString& confFileName = "",
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
   /// Private function drawing the menus of the window
   void drawMenus();

   /// Widget to draw on
   std::unique_ptr<QWidget> m_centralWidget;

   /// Message server listening to the child processes
   msg::Server m_msgServer;
   /// Widget showing the incoming messages
   std::unique_ptr<msg::TextView> m_msgView;

   /// Widget showing DAQ statistics
   std::unique_ptr<simple_daq::Statistics> m_statistics;
   /// Widget controlling cda-caen-vme-reader
   std::unique_ptr<simple_daq::CaenVmeReaderRunner> m_caenVmeReader;
   /// Widget controlling cda-glomem-writer
   std::unique_ptr<simple_daq::GlomemWriterRunner> m_glomemWriter;
   /// Widget controlling cda-hbook-writer
   std::unique_ptr<simple_daq::HBookWriterRunner> m_hbookWriter;
   /// Widget controlling cda-root-writer
   std::unique_ptr<simple_daq::RootWriterRunner> m_rootWriter;
   /// Widget controlling cda-raw-writer
   std::unique_ptr<simple_daq::RawWriterRunner> m_rawWriter;

   /// Description of the extra event receivers
   std::unique_ptr<QLabel> m_eventReceiversLabel;
   /// Widget specifying the extra event receivers
   std::unique_ptr<QComboBox> m_eventReceivers;
   /// Button for removing an event receiver
   std::unique_ptr<QPushButton> m_removeEventReceiver;
   /// Address of the currently selected event receiver
   QString m_currentEventReceiver;

   /// Description of the extra statistics receivers
   std::unique_ptr<QLabel> m_statReceiversLabel;
   /// Widget specifying the extra statistics receivers
   std::unique_ptr<QComboBox> m_statReceivers;
   /// Button for removing an statistics receiver
   std::unique_ptr<QPushButton> m_removeStatReceiver;
   /// Address of the currently selected stat. receiver
   QString m_currentStatReceiver;

   /// Private message logger
   mutable msg::Logger m_logger;

};  // class CaenDAQWindow

#endif  // CDA_APPS_CDA_CAEN_VME_DAQ_CAENDAQWINDOW_H
