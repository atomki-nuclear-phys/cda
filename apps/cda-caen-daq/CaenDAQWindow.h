// Dear emacs, this is -*- c++ -*-
// $Id$
#ifndef CDA_APPS_CDA_CAEN_DAQ_CAENDAQWINDOW_H
#define CDA_APPS_CDA_CAEN_DAQ_CAENDAQWINDOW_H

// Qt include(s):
#include <QtGui/QMainWindow>

// CDA include(s):
#ifdef Q_OS_DARWIN
#   include "cdacore/msg/Logger.h"
#else
#   include "msg/Logger.h"
#endif

// Forward declaration(s):
QT_FORWARD_DECLARE_CLASS( QWidget )
namespace msg {
   class Server;
   class TextView;
} // namespace msg
namespace simple_daq {
   class Statistics;
   class CaenReaderRunner;
   class GlomemWriterRunner;
   class HBookWriterRunner;
   class RootWriterRunner;
   class RawWriterRunner;
} // namespace simple_daq

/**
 *  @short Main (and only) window of cda-caen-daq
 *
 *         This is the main window of cda-caen-daq. It shows widgets that make
 *         it easy to start the cda-camac-reader, cda-glomem-writer and cda-hbook-writer
 *         applications with the click of a button. It is even smart enough to start
 *         these applications with all the correct command line options.
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
   CaenDAQWindow( const QString& confFileName = "",
                  msg::Level verbosity = msg::INFO );
   /// Destructor
   ~CaenDAQWindow();

private slots:
   /// Slot for specifying a configuration file
   void readConfigSlot();
   /// Slot for displaying a window about this application
   void aboutCaenDAQSlot();
   /// Slot for displaying a window about CDA
   void aboutCDASlot();

private:
   /// Private function drawing the menus of the window
   void drawMenus();

   QWidget* m_centralWidget; ///< Widget to draw on

   msg::Server*   m_msgServer; ///< Message server listening to the child processes
   msg::TextView* m_msgView;   ///< Widget showing the incoming messages

   simple_daq::Statistics*         m_statistics;   ///< Widget showing DAQ statistics
   simple_daq::CaenReaderRunner*   m_caenReader;   ///< Widget controlling cda-caen-reader
   simple_daq::GlomemWriterRunner* m_glomemWriter; ///< Widget controlling cda-glomem-writer
   simple_daq::HBookWriterRunner*  m_hbookWriter;  ///< Widget controlling cda-hbook-writer
   simple_daq::RootWriterRunner*   m_rootWriter;   ///< Widget controlling cda-root-writer
   simple_daq::RawWriterRunner*    m_rawWriter;    ///< Widget controlling cda-raw-writer

   mutable msg::Logger m_logger; ///< Private message logger

}; // class CaenDAQWindow

#endif // CDA_APPS_CDA_CAEN_DAQ_CAENDAQWINDOW_H
